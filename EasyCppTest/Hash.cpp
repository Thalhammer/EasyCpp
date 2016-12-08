#include <gtest/gtest.h>
#include <HexEncoding.h>
#include <Hash/SHA512.h>
#include <Hash/SHA256.h>
#include <Hash/SHA1.h>
#include <Hash/MD5.h>
#include <Hash/MD4.h>
#include <Hash/PBKDF2.h>
#include <Hash/PBKDF1.h>

using namespace EasyCpp::Hash;

namespace EasyCppTest
{
	TEST(Hash, SHA1)
	{
		std::string test = "Hallo Welt, wie gehts dir ?";
		std::string sha = EasyCpp::HexEncoding::encode(SHA1::getString(test));

		ASSERT_EQ(std::string("d0c23a3ce9464ec5e5ff96456655bbcf73b73ac6"), sha);
	}

	TEST(Hash, SHA256)
	{
		std::string test = "Hallo Welt, wie gehts dir ?";
		std::string sha = EasyCpp::HexEncoding::encode(SHA256::getString(test));

		ASSERT_EQ(std::string("e91c4fd6e82a1320276f753072c99465178119b85b574af232ce040aac3a6cd5"), sha);
	}

	TEST(Hash, SHA512)
	{
		std::string test = "Hallo Welt, wie gehts dir ?";
		std::string sha = EasyCpp::HexEncoding::encode(SHA512::getString(test));

		ASSERT_EQ(std::string("a985f29602248f1c1a48e20b43fa8dac4ee1bdcea456adc262a596c7ac5b4ade70469a1400bdeea4e11c341104ec31aefa195a2ff620f14d48f4b3368ee24d24"), sha);
	}

	TEST(Hash, MD5)
	{
		std::string test = "Hallo Welt, wie gehts dir ?";
		std::string md5 = EasyCpp::HexEncoding::encode(MD5::getString(test));

		ASSERT_EQ(std::string("8eaf78dea8b35409569144b5ca6aa691"), md5);
		ASSERT_EQ(std::string("d41d8cd98f00b204e9800998ecf8427e"), EasyCpp::HexEncoding::encode(MD5::getString("")));
	}

	TEST(Hash, MD4)
	{
		std::string test = "Hallo Welt, wie gehts dir ?";
		std::string md4 = EasyCpp::HexEncoding::encode(MD4::getString(test));

		ASSERT_EQ(std::string("02c33bc568f0f243b61d186248c4bd3b"), md4);
	}

	TEST(Hash, PBKDF2)
	{
		std::string password = "this is an example";
		std::string salt = "test";
		size_t num_iterations = 1000;

		std::string derive = EasyCpp::HexEncoding::encode(PBKDF2().run(password, salt, num_iterations,32));
		ASSERT_EQ(std::string("141d8d155b3545341be58f51348bf837033219a9177233c0a00cd26d89abec4a"), derive);
	}

	TEST(Hash, PBKDF1)
	{
		std::string password = "this is an example";
		std::string salt = "test";
		size_t num_iterations = 1000;

		std::string derive = EasyCpp::HexEncoding::encode(PBKDF1().run(password, salt, num_iterations));
		ASSERT_EQ(std::string("b6e5e3f483c3d814baf8d91138eed2287bb10e1f"), derive);
	}
}