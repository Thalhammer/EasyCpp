#pragma once
#include "../DllExport.h"
#include "../AnyFunction.h"
#include "../Promise.h"
#include <future>

namespace EasyCpp
{
	namespace Net
	{
		class DLL_EXPORT JsonRPC
		{
		public:
			typedef std::function<void(const AnyValue&)> response_fn_t;
			typedef std::function<void(const AnyValue&, response_fn_t)> rpc_fn_t;
			typedef std::function<void(const AnyValue&, bool)> cb_fn_t;
			typedef std::function<void(const std::string&)> transmit_fn_t;

			JsonRPC();
			~JsonRPC();

			void registerFunction(const std::string& name, rpc_fn_t fn);
			void removeFunction(const std::string& name);
			bool hasFunction(const std::string& name) const;

			void handleMessage(const std::string& msg);

			void callFunction(const std::string& name, const AnyArray& args, cb_fn_t cb);
			void callFunction(const std::string& name, const Serialize::Serializable& args, cb_fn_t cb);
			Promise<AnyValue> callFunction(const std::string& name, const AnyArray& args);
			Promise<AnyValue> callFunction(const std::string& name, const Serialize::Serializable& args);
			void sendNotification(const std::string& name, const AnyArray& args);
			void sendNotification(const std::string& name, const Serialize::Serializable& args);

			void setTransmitCallback(transmit_fn_t fn);
			transmit_fn_t getTransmitCallback() const;

			class Error
			{
			private:
				int _code;
				std::string _message;
				AnyValue _data;
			public:
				Error(int code, const std::string& msg = "", AnyValue data = nullptr);
				int getCode() const;
				const std::string& getMessage() const;
				AnyValue getData() const;
			};
		private:
			std::atomic<uint64_t> _next_id;
			std::map<std::string, rpc_fn_t> _functions;
			std::map<uint64_t, cb_fn_t> _cb_map;
			transmit_fn_t _transmit_fn;

			void handleResponse(const Bundle& data);
			void callFunction(const std::string& name, AnyValue args, cb_fn_t cb);
			void sendNotification(const std::string& name, AnyValue args);

			void sendResult(AnyValue id, AnyValue result);
			void sendResultError(AnyValue id, Error e);
		};
	}
}
