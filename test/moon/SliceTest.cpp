#include <moon/Slice.h>

#include <gtest/gtest.h>

using namespace moon;

TEST(SliceTest, constMethod){
	const char *data = "0123456789";
    Slice slice(data);

	EXPECT_STREQ(data, slice.data());
	EXPECT_EQ(10, slice.size());
	EXPECT_FALSE(slice.empty());
	EXPECT_EQ('0', slice[0]);
	EXPECT_EQ('9', slice[9]);
	EXPECT_STREQ(data, slice.toString().c_str());
}

TEST(SliceTest, clear){
	const char *data = "0123456789";
    Slice slice(data);

	EXPECT_EQ(10, slice.size());

	slice.clear();
	EXPECT_EQ(0, slice.size());
	EXPECT_TRUE(slice.empty());
}

TEST(SliceTest, set){
	const char *data = "0123456789";
    Slice slice(data);

	EXPECT_EQ(10, slice.size());
    EXPECT_STREQ(data, slice.data());
	
	slice.set(data, 5);
	EXPECT_EQ(5, slice.size());
}

TEST(SliceTest, removePrefix){
	const char *data = "0123456789";
    Slice slice(data);

	EXPECT_EQ(10, slice.size());
    EXPECT_STREQ(data, slice.data());
	
	slice.removePrefix(5);
	EXPECT_EQ(5, slice.size());
	EXPECT_STREQ(data + 5, slice.data());
}

TEST(SliceTest, removeSuffix){
	const char *data = "0123456789";
    Slice slice(data);

	EXPECT_EQ(10, slice.size());
    EXPECT_STREQ(data, slice.data());
	
	slice.removeSuffix(5);
	EXPECT_EQ(5, slice.size());
	EXPECT_EQ(0, memcmp(data, slice.data(), 5) );
}

TEST(SliceTest, startsWith){
	const char *data = "0123456789";
    Slice slice(data);

	EXPECT_TRUE(slice.startsWith("01234"));
	EXPECT_TRUE(slice.startsWith("0123456789"));

	EXPECT_FALSE(slice.startsWith("1234"));
	EXPECT_FALSE(slice.startsWith("01234567890"));
}

TEST(SliceTest, compare){
	const char *data = "0123456789";
    Slice slice(data);

	EXPECT_EQ(-1, slice.compare("1") );
	EXPECT_EQ(0, slice.compare("0123456789") );
	EXPECT_EQ(1, slice.compare("012345678") );
}

TEST(SliceTest, operators){
	const char *data = "0123456789";
    Slice slice(data);

	EXPECT_TRUE(slice == data);
	EXPECT_TRUE(slice >= data);
	EXPECT_TRUE(slice <= data);

	EXPECT_TRUE(slice > "012345678");
	EXPECT_TRUE(slice < "1");	
}

