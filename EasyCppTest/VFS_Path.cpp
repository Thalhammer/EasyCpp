#include <gtest/gtest.h>
#include <VFS/Path.h>

using namespace EasyCpp::VFS;

namespace EasyCppTest
{
	TEST(VFS, PathEmpty)
	{
		Path p;
		ASSERT_EQ("/", p.getString());
		ASSERT_EQ("", p.getBaseName());
		ASSERT_EQ("", p.getExtension());
		ASSERT_EQ("", p.getFileName());
		ASSERT_EQ("/", p.getDirName());
	}

	TEST(VFS, PathFile)
	{
		Path p("/test.bin");
		ASSERT_EQ("/test.bin", p.getString());
		ASSERT_EQ("test.bin", p.getBaseName());
		ASSERT_EQ("bin", p.getExtension());
		ASSERT_EQ("test", p.getFileName());
		ASSERT_EQ("/", p.getDirName());
	}

	TEST(VFS, PathDotFile)
	{
		Path p("/.htaccess");
		ASSERT_EQ("/.htaccess", p.getString());
		ASSERT_EQ(".htaccess", p.getBaseName());
		ASSERT_EQ("htaccess", p.getExtension());
		ASSERT_EQ("", p.getFileName());
		ASSERT_EQ("/", p.getDirName());
	}

	TEST(VFS, PathSubDirectory)
	{
		Path p("/test/123/123");
		ASSERT_EQ("/test/123/123", p.getString());
		ASSERT_EQ("123", p.getBaseName());
		ASSERT_EQ("", p.getExtension());
		ASSERT_EQ("123", p.getFileName());
		ASSERT_EQ("/test/123/", p.getDirName());
	}
}