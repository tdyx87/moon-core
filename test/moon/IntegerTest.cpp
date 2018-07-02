#include <moon/Integer.h>

#include <gtest/gtest.h>

using namespace moon;

TEST(IntegerTest, parseInt){
	EXPECT_EQ(Integer::parseInt("0"), 0);
	EXPECT_EQ(Integer::parseInt("1"), 1);
	EXPECT_EQ(Integer::parseInt("9"), 9);
	EXPECT_EQ(Integer::parseInt("11"), 11);

	EXPECT_EQ(Integer::parseInt("+1"), 1);
	EXPECT_EQ(Integer::parseInt("-1"), -1);

	EXPECT_EQ(Integer::parseInt(" 1"), 1);
	EXPECT_EQ(Integer::parseInt(" 1 "), 1);

	EXPECT_EQ(Integer::parseInt(""), 0);
	EXPECT_EQ(Integer::parseInt("", 1), 1);
	EXPECT_EQ(Integer::parseInt("abc"), 0);
	EXPECT_EQ(Integer::parseInt("abc", 1), 1);

	EXPECT_EQ(Integer::parseInt("12abc"), 12);
	EXPECT_EQ(Integer::parseInt("a12bc"), 0);

	EXPECT_EQ(Integer::parseInt("3333333333333333333"), Integer::MAX_VALUE);
	EXPECT_EQ(Integer::parseInt("-3333333333333333333"), Integer::MIN_VALUE);
}

TEST(IntegerTest, toString){
	EXPECT_EQ(Integer::toString(0), "0");
	EXPECT_EQ(Integer::toString(9), "9");
	EXPECT_EQ(Integer::toString(10), "10");

	EXPECT_EQ(Integer::toString(-10), "-10");
	EXPECT_EQ(Integer::toString(+10), "10");
}
