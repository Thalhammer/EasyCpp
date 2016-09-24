#include <gtest/gtest.h>
#include <Net/WebsocketClient.h>
#include <mutex>
#include <condition_variable>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(WebsocketClient, EchoTest)
	{
		Net::WebsocketClient client;

		client.onOpen([&client]() {
			client.send("Hello");
		});

		bool executed = false;
		std::mutex wait_mtx;
		std::condition_variable cv;
		client.onMessage([&executed, &wait_mtx, &cv](const auto& msg, bool bin) {
			std::unique_lock<std::mutex> lck(wait_mtx);
			ASSERT_EQ("Hello", msg);
			ASSERT_EQ(false, bin);
			executed = true;
			cv.notify_all();
		});

		client.onError([](auto ex) {
			FAIL();
		});

		client.onClose([](auto code, const auto& msg) {
			ASSERT_EQ(1001, code);
		});


		client.connect("wss://echo.websocket.org");

		{
			std::unique_lock<std::mutex> lck(wait_mtx);
			cv.wait(lck);
			//cv.wait_for(lck, std::chrono::seconds(5));
			ASSERT_EQ(true, executed);
		}
	}
	
	TEST(WebsocketClient, ConnectFail)
	{
		Net::WebsocketClient client;

		client.onOpen([]() {
			FAIL();
		});

		client.onMessage([](const auto& msg, bool bin) {
			FAIL();
		});

		client.onError([](auto ex) {
			FAIL();
		});

		client.onClose([](auto code, const auto& msg) {
			FAIL();
		});
		
		ASSERT_THROW({
			client.connect("wss://127.0.0.1/");
		}, std::exception);
	}
}