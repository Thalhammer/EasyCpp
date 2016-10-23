#include <gtest/gtest.h>
#include <Net/WSJsonRPC.h>
#include <Bundle.h>
#include <vector>

using namespace EasyCpp::Net;
using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(JsonRPC, HandleMessage)
	{
		bool fn_executed = false;
		bool send_executed = false;
		JsonRPC rpc;
		auto send_fn = [&send_executed](const std::string& str) {
			send_executed = true;
			ASSERT_EQ(std::string("{\"id\":1,\"jsonrpc\":\"2.0\",\"result\":4}\n"), str);
		};

		rpc.setTransmitCallback(send_fn);
		rpc.registerFunction("add", [&fn_executed](const AnyValue& params, auto reply) {

			ASSERT_TRUE(params.isType<std::vector<AnyValue>>());
			auto args = params.as<std::vector<AnyValue>>();
			ASSERT_TRUE(args.size() == 2);
			ASSERT_TRUE(args[0].isConvertibleTo<int>());
			ASSERT_TRUE(args[1].isConvertibleTo<int>());
			int p1 = args[0].as<int>();
			int p2 = args[1].as<int>();

			fn_executed = true;

			reply(p1 + p2);
		});

		ASSERT_NO_THROW({
			rpc.handleMessage("{\"jsonrpc\": \"2.0\", \"method\": \"add\", \"params\": [2, 2], \"id\": 1}");
		});
		ASSERT_TRUE(fn_executed);
		ASSERT_TRUE(send_executed);
	}

	TEST(JsonRPC, HandleBatchMessage)
	{
		bool fn_1_executed = false;
		bool fn_2_executed = false;
		bool send_executed = false;
		JsonRPC rpc;

		rpc.setTransmitCallback([&send_executed](const std::string& s) {
			ASSERT_EQ(s, "[{\"id\":1,\"jsonrpc\":\"2.0\",\"result\":4},{\"id\":2,\"jsonrpc\":\"2.0\",\"result\":2}]\n");
			send_executed = true;
		});

		rpc.registerFunction("fn1", [&fn_1_executed](const AnyValue& params, auto reply) {

			ASSERT_TRUE(params.isType<std::vector<AnyValue>>());
			auto args = params.as<std::vector<AnyValue>>();
			ASSERT_TRUE(args.size() == 2);
			ASSERT_TRUE(args[0].isConvertibleTo<int>());
			ASSERT_TRUE(args[1].isConvertibleTo<int>());
			int p1 = args[0].as<int>();
			int p2 = args[1].as<int>();

			fn_1_executed = true;

			reply(p1 + p2);
		});
		rpc.registerFunction("fn2", [&fn_2_executed](const AnyValue& params, auto reply) {

			ASSERT_TRUE(params.isType<std::vector<AnyValue>>());
			auto args = params.as<std::vector<AnyValue>>();
			ASSERT_TRUE(args.size() == 2);
			ASSERT_TRUE(args[0].isConvertibleTo<int>());
			ASSERT_TRUE(args[1].isConvertibleTo<int>());
			int p1 = args[0].as<int>();
			int p2 = args[1].as<int>();

			fn_2_executed = true;

			reply(p1 + p2);
		});

		ASSERT_NO_THROW({
			rpc.handleMessage("[{\"jsonrpc\": \"2.0\", \"method\": \"fn1\", \"params\": [2, 2], \"id\": 1},{\"jsonrpc\": \"2.0\", \"method\":\"fn2\", \"params\":[1, 1], \"id\":2}]");
		});
		ASSERT_TRUE(fn_1_executed);
		ASSERT_TRUE(fn_2_executed);
		ASSERT_TRUE(send_executed);
	}

	TEST(JsonRPC, SendNotification)
	{
		JsonRPC rpc;
		bool called = false;
		rpc.setTransmitCallback([&called](const std::string& str) {
			called = true;
		});

		rpc.sendNotification("test", { std::string("Test1"),std::string("Test2") }); // vector<AnyValue>
		ASSERT_EQ(true, called);
	}

	TEST(JsonRPC, DISABLED_WebsocketTest)
	{
		bool call_executed = false;
		bool error_executed = false;

		std::mutex _mtx;
		std::condition_variable _cv;
		WSJsonRPC rpc;
		rpc.onOpen([&]() {
			rpc.callFunction("Server.getSupportedMethods", Bundle()).then(std::function<void(EasyCpp::AnyValue& val)>([&](const EasyCpp::AnyValue& res) {
				call_executed = true;
				ASSERT_TRUE(res.isType<AnyArray>());
				std::unique_lock<std::mutex> lck(_mtx);
				_cv.notify_all();
			}));
		});
		rpc.onError([&error_executed](std::exception_ptr ex) {
			error_executed = true;
		});
		rpc.connect("wss://localhost.systemserver.tk:666/system");
		{
			std::unique_lock<std::mutex> lck(_mtx);
			_cv.wait_for(lck, std::chrono::seconds(10));
		}
		ASSERT_TRUE(call_executed);
		ASSERT_FALSE(error_executed);
	}

	TEST(JsonRPC, CallFunctionSend)
	{
		JsonRPC rpc;
		bool called = false;
		rpc.setTransmitCallback([&called](const std::string& str) {
			called = true;
		});

		rpc.callFunction("test", { std::string("Test1"),std::string("Test2") }, [](const EasyCpp::AnyValue& val, bool error) {}); // vector<AnyValue>
		ASSERT_EQ(true, called);
	}

}
