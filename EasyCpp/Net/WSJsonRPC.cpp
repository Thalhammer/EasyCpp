#include "WSJsonRPC.h"

namespace EasyCpp
{
	namespace Net
	{
		WSJsonRPC::WSJsonRPC()
		{
			_rpc.setTransmitCallback([this](const std::string& msg) {
				this->_connection.send(msg, false);
			});
			_connection.onMessage([this](const std::string& msg, bool bin) {
				if (bin)
					_on_binary(msg);
				else _rpc.handleMessage(msg);
			});
		}

		WSJsonRPC::~WSJsonRPC()
		{
		}

		void WSJsonRPC::registerFunction(const std::string & name, JsonRPC::rpc_fn_t fn)
		{
			_rpc.registerFunction(name, fn);
		}

		void WSJsonRPC::removeFunction(const std::string & name)
		{
			_rpc.removeFunction(name);
		}

		bool WSJsonRPC::hasFunction(const std::string & name) const
		{
			return _rpc.hasFunction(name);
		}

		void WSJsonRPC::callFunction(const std::string & name, const AnyArray & args, JsonRPC::cb_fn_t cb)
		{
			_rpc.callFunction(name, args, cb);
		}

		void WSJsonRPC::callFunction(const std::string & name, const Serialize::Serializable & args, JsonRPC::cb_fn_t cb)
		{
			_rpc.callFunction(name, args, cb);
		}

		Promise<AnyValue> WSJsonRPC::callFunction(const std::string & name, const AnyArray & args)
		{
			return _rpc.callFunction(name, args);
		}

		Promise<AnyValue> WSJsonRPC::callFunction(const std::string & name, const Serialize::Serializable & args)
		{
			return _rpc.callFunction(name, args);
		}

		void WSJsonRPC::sendNotification(const std::string & name, const AnyArray & args)
		{
			_rpc.sendNotification(name, args);
		}

		void WSJsonRPC::sendNotification(const std::string & name, const Serialize::Serializable & args)
		{
			_rpc.sendNotification(name, args);
		}

		void WSJsonRPC::onOpen(const std::function<void()>& fn)
		{
			_connection.onOpen(fn);
		}

		void WSJsonRPC::onClose(const std::function<void(uint16_t, const std::string&)>& fn)
		{
			_connection.onClose([fn, this](uint16_t code, const std::string& msg) {
				fn(code, msg);
				_rpc.resetCalls(std::to_string(code) + ":" + msg);
			});
		}

		void WSJsonRPC::onError(const std::function<void(std::exception_ptr)>& fn)
		{
			_connection.onError(fn);
		}

		void WSJsonRPC::onMessage(const std::function<void(const std::string&)>& fn)
		{
			_on_binary = fn;
		}

		void WSJsonRPC::send(const std::string & msg)
		{
			_connection.send(msg, true);
		}

		void WSJsonRPC::connect(const std::string & url)
		{
			_connection.connect(url);
		}

		void WSJsonRPC::disconnect(uint16_t code, const std::string & msg)
		{
			_connection.disconnect(code, msg);
		}
	}
}