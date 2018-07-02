#include <moon/Atomic.h>

#include <gtest/gtest.h>

using namespace moon;

class AtomicTest  : public testing::Test
{
public:
	AtomicTest() : seq(1) {}
public:	
	Atomic<int> seq;
};

TEST_F(AtomicTest, get){	
	EXPECT_EQ(seq.get(), 1);	
}

TEST_F(AtomicTest, set){
	seq.set(3);	
	EXPECT_EQ(seq.get(), 3);	
}

TEST_F(AtomicTest, fetchAndAdd){
	int oldvalue = seq.fetchAndAdd(2);	
	
	EXPECT_EQ(oldvalue, 1);	
	EXPECT_EQ(seq.get(), 3);	
}

TEST_F(AtomicTest, fetchAndSub){
	int oldvalue = seq.fetchAndSub(2);	
	
	EXPECT_EQ(oldvalue, 1);	
	EXPECT_EQ(seq.get(), -1);	
}

TEST_F(AtomicTest, fetchAndOr){
	int oldvalue = seq.fetchAndOr(0);	
	
	EXPECT_EQ(oldvalue, 1);	
	EXPECT_EQ(seq.get(), 1);	

	oldvalue = seq.fetchAndOr(2);	
	EXPECT_EQ(oldvalue, 1);	
	EXPECT_EQ(seq.get(), 3);	
}

TEST_F(AtomicTest, fetchAndAnd){
	int oldvalue = seq.fetchAndAnd(0);	
	
	EXPECT_EQ(oldvalue, 1);	
	EXPECT_EQ(seq.get(), 0);	

	seq.set(1);
	oldvalue = seq.fetchAndAnd(3);	
	EXPECT_EQ(oldvalue, 1);	
	EXPECT_EQ(seq.get(), 1);	
}

TEST_F(AtomicTest, fetchAndXor){
	int oldvalue = seq.fetchAndXor(0);	
	
	EXPECT_EQ(oldvalue, 1);	
	EXPECT_EQ(seq.get(), 1);	

	oldvalue = seq.fetchAndXor(1);	
	EXPECT_EQ(oldvalue, 1);	
	EXPECT_EQ(seq.get(), 0);	
}

TEST_F(AtomicTest, addAndFetch){
	int ret = seq.addAndFetch(2);	
	
	EXPECT_EQ(ret, 3);	
	EXPECT_EQ(seq.get(), 3);	
}

TEST_F(AtomicTest, subAndFetch){
	int ret = seq.subAndFetch(2);	
	
	EXPECT_EQ(ret, -1);	
	EXPECT_EQ(seq.get(), -1);	
}

TEST_F(AtomicTest, orAndFetch){
	int ret = seq.orAndFetch(0);	
	
	EXPECT_EQ(ret, 1);	
	EXPECT_EQ(seq.get(), 1);	

	ret = seq.orAndFetch(2);	
	EXPECT_EQ(ret, 3);	
	EXPECT_EQ(seq.get(), 3);	
}

TEST_F(AtomicTest, andAndFetch){
	int ret = seq.andAndFetch(0);	
	
	EXPECT_EQ(ret, 0);	
	EXPECT_EQ(seq.get(), 0);	

	seq.set(1);
	ret = seq.andAndFetch(3);	
	EXPECT_EQ(ret, 1);	
	EXPECT_EQ(seq.get(), 1);	
}

TEST_F(AtomicTest, xorAndFetch){
	int ret = seq.xorAndFetch(0);	
	
	EXPECT_EQ(ret, 1);	
	EXPECT_EQ(seq.get(), 1);	

	ret = seq.xorAndFetch(1);	
	EXPECT_EQ(ret, 0);	
	EXPECT_EQ(seq.get(), 0);	
}

