#include <moon/Double.h>

#include <gtest/gtest.h>

using namespace moon;

TEST(DoubleTest, parseDouble){
	EXPECT_EQ(Double::compare(Double::parseDouble("0"), 0.00), 0);
	EXPECT_EQ(Double::compare(Double::parseDouble("1.000"), 1.000), 0);
	EXPECT_EQ(Double::compare(Double::parseDouble("1.000001"), 1.000001), 0);
}

TEST(DoubleTest, compare){
	EXPECT_EQ(Double::compare(1.001, 1.00001), 1);
	EXPECT_EQ(Double::compare(1.00001, 1.00001), 0);
	EXPECT_EQ(Double::compare(1.00001, 1.001), -1);
}

TEST(DoubleTest, toString){
	EXPECT_EQ(Double::toString(1.001), "1.001");
	EXPECT_EQ(Double::toString(1.000010), "1.00001");
	//EXPECT_EQ(Double::toString(0.00001), "le-05");
}
