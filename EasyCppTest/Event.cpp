#include <gtest/gtest.h>
#include <Event.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(Event, Event)
	{
		bool executed = false;

		Event<bool> ev;

		auto con = ev.registerConnection();
		con->setFunction([&executed](bool arg) {
			executed = true;
		});

		ASSERT_EQ(1, ev.getNumConnections());

		ev(true);

		ASSERT_TRUE(executed);
	}

	TEST(Event, DestroyEvent)
	{
		std::shared_ptr<EventConnection<bool>> con;
		{
			Event<bool> ev;
			con = ev.registerConnection();
		}
	}

	TEST(Event, DestroyConnnection)
	{
		Event<bool> ev;
		{
			auto con = ev.registerConnection();
			ASSERT_EQ(1, ev.getNumConnections());
		}
		ASSERT_EQ(0, ev.getNumConnections());

		ev(true);
	}
}