#include "JsonRPC.h"
#include "../Serialize/JsonSerializer.h"
#include <cstddef>

namespace EasyCpp
{
	namespace Net
	{
		JsonRPC::JsonRPC()
			:_next_id(0)
		{
		}

		JsonRPC::~JsonRPC()
		{
		}

		void JsonRPC::registerFunction(const std::string & name, rpc_fn_t fn)
		{
			if (_functions.count(name) != 0)
				throw std::runtime_error("Function already registered");
			_functions.insert({ name, fn });
		}

		void JsonRPC::removeFunction(const std::string & name)
		{
			_functions.erase(name);
		}

		bool JsonRPC::hasFunction(const std::string & name) const
		{
			return _functions.count(name) != 0;
		}

		void JsonRPC::handleMessage(const std::string & msg)
		{
			try {
				AnyValue data;
				Serialize::JsonSerializer json;
				// Parse Request
				try {
					data = json.deserialize(msg);
					if (!data.isType<Bundle>() && !data.isType<AnyArray>())
						throw std::runtime_error("Invalid type received");
				}
				catch (const std::exception& e)
				{
					throw Error(-32700, "Failed to parse request");
				}

				if (data.isType<Bundle>()) {
					// Single request
					auto bundle = data.as<Bundle>();
					if (bundle.isSet("method")) {
						AnyValue id = bundle.get("id");
						if (_functions.count(bundle.get<std::string>("method")) != 1)
							throw Error(-32601, "Method not found", nullptr, id);
						_functions.at(bundle.get<std::string>("method"))(bundle.get("params"), [id, this](const AnyValue& result){
							if (result.isType<Error>()) {
								this->sendResultError(id, result.as<Error&>());
							} else {
								this->sendResult(id, result);
							}
						});
					} else {
						this->handleResponse(bundle);
					}
				} else {
					auto req = data.as<AnyArray>();
					auto map = std::make_shared<std::vector<std::pair<AnyValue, AnyValue>>>();
					uint64_t num_reply = req.size();
					for (auto& e : req) {
						if (!e.isType<Bundle>())
							throw Error(-32600, "Invalid request");

						Bundle bundle = e.as<Bundle>();
						if (!bundle.isSet("method"))
						{
							this->handleResponse(bundle);
						}

						AnyValue id = bundle.isSet("id")?bundle.get("id"):AnyValue();
						std::string method = bundle.get<std::string>("method");
						if (_functions.count(method) != 1) {
							map->push_back({ id, AnyValue(Error(-32601, "Method not found")) });
							if (map->size() == num_reply) // All requests replied
							{
								if (!_transmit_fn) return;
								AnyArray results;
								for (auto &e : *map)
								{
									if (e.first.isType<nullptr_t>())
										continue;
									if (e.second.isType<Error>()) {
										Error ex = e.second.as<Error&>();
										results.push_back(Bundle({
											{ "jsonrpc", "2.0" },
											{ "id", e.first },
											{ "error", Bundle({
												{ "code", ex.getCode() },
												{ "message", ex.getMessage() },
												{ "data", ex.getData() }
										}) }
										}));
									}
									else {
										results.push_back(Bundle({
											{ "jsonrpc", "2.0" },
											{ "id", e.first },
											{ "result", e.second }
										}));
									}
								}
								std::string str = Serialize::JsonSerializer().serialize(results);
								_transmit_fn(str);
							}
						}
						else {
							_functions.at(method)(bundle.isSet("params") ? bundle.get("params") : AnyValue(), [id, num_reply, map, this](const AnyValue& result) {
								map->push_back({ id, result });
								if (map->size() == num_reply) // All requests replied
								{
									if (!_transmit_fn) return;
									AnyArray results;
									for (auto &e : *map)
									{
										if (e.first.isType<nullptr_t>())
											continue;
										if (e.second.isType<Error>()) {
											Error ex = e.second.as<Error&>();
											results.push_back(Bundle({
												{ "jsonrpc", "2.0" },
												{ "id", e.first },
												{ "error", Bundle({
													{ "code", ex.getCode() },
													{ "message", ex.getMessage() },
													{ "data", ex.getData() }
												}) }
											}));
										}
										else {
											results.push_back(Bundle({
												{ "jsonrpc", "2.0" },
												{ "id", e.first },
												{ "result", e.second }
											}));
										}
									}
									std::string str = Serialize::JsonSerializer().serialize(results);
									_transmit_fn(str);
								}
							});
						}
					}
				}
			}catch(const Error& e)
			{
				this->sendResultError(e.getRequestId(), e);
			}
		}

		void JsonRPC::callFunction(const std::string & name, const AnyArray & args, cb_fn_t cb)
		{
			this->callFunction(name, AnyValue(args), cb);
		}

		void JsonRPC::callFunction(const std::string & name, const Serialize::Serializable & args, cb_fn_t cb)
		{
			this->callFunction(name, args.toAnyValue(), cb);
		}

		Promise<AnyValue> JsonRPC::callFunction(const std::string & name, const AnyArray & args)
		{
			Promise<AnyValue> promise;
			this->callFunction(name, args, [promise](const AnyValue& res, bool error) mutable {
				try {
					if (error) {
						EasyCpp::Bundle e = res.as<EasyCpp::Bundle>();
						throw Error(e.get<int>("code"), e.get<std::string>("message"), e.isSet("data") ? e.get("data") : EasyCpp::AnyValue());
					}
					promise.resolve(res);
				}catch(...) {
					promise.reject(std::current_exception());
				}
			});
			return promise;
		}

		Promise<AnyValue> JsonRPC::callFunction(const std::string & name, const Serialize::Serializable & args)
		{
			Promise<AnyValue> promise;
			this->callFunction(name, args, [promise](const AnyValue& res, bool error) mutable {
				try {
					if (error) {
						EasyCpp::Bundle e = res.as<EasyCpp::Bundle>();
						throw Error(e.get<int>("code"), e.get<std::string>("message"), e.isSet("data") ? e.get("data") : EasyCpp::AnyValue());
					}
					promise.resolve(res);
				}
				catch (...) {
					promise.reject(std::current_exception());
				}
			});
			return promise;
		}

		void JsonRPC::sendNotification(const std::string & name, const AnyArray & args)
		{
			this->sendNotification(name, AnyValue(args));
		}

		void JsonRPC::sendNotification(const std::string & name, const Serialize::Serializable & args)
		{
			this->sendNotification(name, args.toAnyValue());
		}

		void JsonRPC::setTransmitCallback(transmit_fn_t fn)
		{
			_transmit_fn = fn;
		}
		JsonRPC::transmit_fn_t JsonRPC::getTransmitCallback() const
		{
			return _transmit_fn;
		}

		void JsonRPC::resetCalls(const std::string & reason)
		{
			Bundle error({
				{ "code", -1 },
				{ "message", std::string(reason) },
				{ "data", Bundle() }
			});
			for (auto& e : _cb_map)
			{
				e.second(error, false);
			}
			_cb_map.clear();
		}

		void JsonRPC::handleResponse(const Bundle & data)
		{
			uint64_t id = data.get<uint64_t>("id");
			if (_cb_map.count(id) != 0)
			{
				try {
					if (data.isSet("error"))
						_cb_map.at(id)(data.get("error"), true);
					else
						_cb_map.at(id)(data.get("result"), false);
				} catch(const std::exception& e) {}
			}
		}

		void JsonRPC::callFunction(const std::string & name, AnyValue args, cb_fn_t cb)
		{
			if (!_transmit_fn) return;
			int64_t id = _next_id.fetch_add(1);
			Bundle req({
				{ "jsonrpc", "2.0" },
				{ "method", std::string(name) },
				{ "id", id },
				{ "params", args }
			});

			std::string str = Serialize::JsonSerializer().serialize(req);
			_transmit_fn(str);
			_cb_map.insert({ id, cb });
		}

		void JsonRPC::sendNotification(const std::string & name, AnyValue args)
		{
			if (!_transmit_fn) return;
			Bundle req({
				{ "jsonrpc", "2.0" },
				{ "method", std::string(name) },
				{ "params", args }
			});

			std::string str = Serialize::JsonSerializer().serialize(req);
			_transmit_fn(str);
		}

		void JsonRPC::sendResult(AnyValue id, AnyValue result)
		{
			if (!_transmit_fn) return;
			if (id.isType<nullptr_t>()) return;
			Bundle req({
				{ "jsonrpc", "2.0" },
				{ "id", id },
				{ "result", result }
			});

			std::string str = Serialize::JsonSerializer().serialize(req);
			_transmit_fn(str);
		}

		void JsonRPC::sendResultError(AnyValue id, Error e)
		{
			if (!_transmit_fn) return;
			if (id.isType<nullptr_t>()) return;
			Bundle req({
				{ "jsonrpc", "2.0" },
				{ "id", id },
				{ "error", Bundle({
					{ "code", e.getCode() },
					{ "message", e.getMessage() },
					{ "data", e.getData() }
				})}
			});

			std::string str = Serialize::JsonSerializer().serialize(req);
			_transmit_fn(str);
		}

		JsonRPC::Error::Error(int code, const std::string & msg, AnyValue data, AnyValue reqid)
			:_code(code), _message(msg), _data(data), _reqid(reqid)
		{
		}

		int JsonRPC::Error::getCode() const
		{
			return _code;
		}

		const std::string & JsonRPC::Error::getMessage() const
		{
			return _message;
		}

		AnyValue JsonRPC::Error::getData() const
		{
			return _data;
		}
		AnyValue JsonRPC::Error::getRequestId() const
		{
			return _reqid;
		}
	}
}
