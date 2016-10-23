#pragma once
#include "JsonRPC.h"
#include "WebsocketClient.h"

namespace EasyCpp
{
	namespace Net
	{
		class DLL_EXPORT WSJsonRPC
		{
		public:
			WSJsonRPC();
			~WSJsonRPC();

			void registerFunction(const std::string& name, JsonRPC::rpc_fn_t fn);
			void removeFunction(const std::string& name);
			bool hasFunction(const std::string& name) const;

			void callFunction(const std::string& name, const AnyArray& args, JsonRPC::cb_fn_t cb);
			void callFunction(const std::string& name, const Serialize::Serializable& args, JsonRPC::cb_fn_t cb);
			Promise<AnyValue> callFunction(const std::string& name, const AnyArray& args);
			Promise<AnyValue> callFunction(const std::string& name, const Serialize::Serializable& args);
			void sendNotification(const std::string& name, const AnyArray& args);
			void sendNotification(const std::string& name, const Serialize::Serializable& args);

			void onOpen(const std::function<void()>& fn);
			void onClose(const std::function<void(uint16_t, const std::string&)>& fn);
			void onError(const std::function<void(std::exception_ptr)>& fn);
			// Called on Binary messages only
			void onMessage(const std::function<void(const std::string&)>& fn);
			// Send a binary message
			void send(const std::string& msg);

			void connect(const std::string& url);
			void disconnect(uint16_t code = 1001, const std::string& msg = "Going away");
		private:
			JsonRPC _rpc;
			WebsocketClient _connection;

			std::function<void(const std::string&)> _on_binary;
		};
	}
}
