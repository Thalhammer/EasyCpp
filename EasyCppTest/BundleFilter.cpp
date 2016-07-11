#include <gtest/gtest.h>
#include <BundleFilter.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(BundleFilter, BasicFilter)
	{
		Bundle sample({
			{ "test1", "teststr" },
			{ "test2", "other str" },
			{ "testb", Bundle({
				{ "subkey", "Test" },
				{ "subkey2", "Test2" }
			}) }
		});

		BundleFilter filter("{test2, testb{subkey2}}");
		Bundle result = filter.filterBundle(sample);

		ASSERT_EQ("other str", result.get<std::string>("test2"));
		ASSERT_EQ("Test2", result.get<Bundle>("testb").get<std::string>("subkey2"));
	}

	TEST(BundleFilter, BadFilterString)
	{
		Bundle sample({
			{ "test1", "teststr" },
			{ "test2", "other str" },
			{ "testb", Bundle({
				{ "subkey", "Test" },
				{ "subkey2", "Test2" }
			}) }
		});


		ASSERT_THROW({
			BundleFilter filter("{test2, testb{subkey2}}}");
			filter.filterBundle(sample);
		}, std::exception);
		ASSERT_THROW({
			BundleFilter filter("{test2, testb{{subkey2}}");
			filter.filterBundle(sample);
		}, std::exception);
	}
}