#include <moon/Status.h>

#include <gtest/gtest.h>

using namespace moon;

TEST(StatusTest, copyConstructor){
	Status status = Status::error(-1, "error", "copy operator test");
	Status status1(status);

    EXPECT_FALSE(status1);
	EXPECT_FALSE(status1.isOk());
	 
	EXPECT_EQ(-1, status.errorCode());
	EXPECT_STREQ("error: copy operator test", status1.error());	
	EXPECT_STREQ("error: copy operator test", status1.errorString().c_str());	
}

TEST(StatusTest, assignOperator){
	Status status = Status::error(-1, "error", "copy operator test");
	Status status1 = status;

    EXPECT_FALSE(status1);
	EXPECT_FALSE(status1.isOk());
	 
	EXPECT_EQ(-1, status.errorCode());
	EXPECT_STREQ("error: copy operator test", status1.error());	
	EXPECT_STREQ("error: copy operator test", status1.errorString().c_str());	
}


