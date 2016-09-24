#pragma once
#include "../DllExport.h"
#include "../NonCopyable.h"
#include "Curl.h"
#include <thread>
#include <atomic>
#include <memory>

namespace EasyCpp
{
	namespace Net
	{
		class DLL_EXPORT WebsocketClient : public NonCopyable
		{
		public:
			WebsocketClient();
			~WebsocketClient();

			void onOpen(const std::function<void()>& fn);
			void onClose(const std::function<void(uint16_t, const std::string&)>& fn);
			void onError(const std::function<void(std::exception_ptr)>& fn);
			void onMessage(const std::function<void(const std::string&, bool)>& fn);

			void send(const std::string& msg, bool bin = false);

			void connect(const std::string& url);
			void disconnect(uint16_t code = 1001, const std::string& msg = "Going away");
		private:
			std::unique_ptr<Curl> _curl;

			std::atomic<bool> _exit;
			std::thread _read_thread;

			std::function<void()> _on_open;
			std::function<void(uint16_t, const std::string&)> _on_close;
			std::function<void(std::exception_ptr)> _on_error;
			std::function<void(const std::string&, bool)> _on_message;

			struct frame;
			enum steps {
				READ_START,
				READ_LEN,
				READ_CONTENT
			};
			struct parser_state;
			void sendAll(const std::string& data);
			void sendFrame(const frame& frame);
			void onFrame(const frame& frame);
		};
	}
}
