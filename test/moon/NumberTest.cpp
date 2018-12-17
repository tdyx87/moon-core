#include <moon/Number.h>

#include <gtest/gtest.h>

using namespace moon;

TEST(NumberTest, parseInt){
	EXPECT_EQ(Number::parseInt("0"), 0);
	EXPECT_EQ(Number::parseInt("1"), 1);
	EXPECT_EQ(Number::parseInt("9"), 9);
	EXPECT_EQ(Number::parseInt("11"), 11);

	EXPECT_EQ(Number::parseInt("+1"), 1);
	EXPECT_EQ(Number::parseInt("-1"), -1);

	EXPECT_EQ(Number::parseInt(" 1"), 1);
	EXPECT_EQ(Number::parseInt(" 1 "), 1);

	EXPECT_EQ(Number::parseInt(""), 0);
	EXPECT_EQ(Number::parseInt("", 1), 1);
	EXPECT_EQ(Number::parseInt("abc"), 0);
	EXPECT_EQ(Number::parseInt("abc", 1), 1);

	EXPECT_EQ(Number::parseInt("12abc"), 12);
	EXPECT_EQ(Number::parseInt("a12bc"), 0);

	EXPECT_EQ(Number::parseInt("3333333333333333333"), Number::kIntMax);
	EXPECT_EQ(Number::parseInt("-3333333333333333333"), Number::kIntMin);
}

TEST(NumberTest, intToString){
	EXPECT_EQ(Number::intToString(0), "0");
	EXPECT_EQ(Number::intToString(9), "9");
	EXPECT_EQ(Number::intToString(10), "10");

	EXPECT_EQ(Number::intToString(-10), "-10");
	EXPECT_EQ(Number::intToString(+10), "10");
}

TEST(NumberTest, parseDouble){
	EXPECT_EQ(Number::compare(Number::parseDouble("0"), 0.00), 0);
	EXPECT_EQ(Number::compare(Number::parseDouble("1.000"), 1.000), 0);
	EXPECT_EQ(Number::compare(Number::parseDouble("1.000001"), 1.000001), 0);
}

TEST(NumberTest, compare){
	EXPECT_EQ(Number::compare(1.001, 1.00001), 1);
	EXPECT_EQ(Number::compare(1.00001, 1.00001), 0);
	EXPECT_EQ(Number::compare(1.00001, 1.001), -1);
}

TEST(NumberTest, doubleToString){
	EXPECT_EQ(Number::doubleToString(1.001), "1.001");
	EXPECT_EQ(Number::doubleToString(1.000010), "1.00001");
}
