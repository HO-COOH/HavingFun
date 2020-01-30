#include <gtest/gtest.h>
#include <iostream>

struct Accout
{
	int balance;
};

struct TestFixture: testing::Test
{
	Accout a;
	//TestFixture() {}
};

TEST_F(TestFixture, AccountInitZero)
{
	EXPECT_EQ(0, a.balance);	//should fail because balance is not initialized!
}

TEST(TestNonFixture, AccountInitZero)
{
	Accout b;
	EXPECT_EQ(0, b.balance);	//should fail because balance is not initialized!
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}