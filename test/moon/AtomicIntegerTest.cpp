#include <moon/AtomicInteger.h>

#include <gtest/gtest.h>

using namespace moon;

class AtomicIntegerTest  : public testing::Test
{
public:
	AtomicIntegerTest() : seq(1) {}
public:	
	AtomicInt32 seq;
};

TEST_F(AtomicIntegerTest, get){	
	EXPECT_EQ(1, seq.get());	
}

TEST_F(AtomicIntegerTest, getAndAdd){
    int32_t ret = seq.getAndAdd(2);
	EXPECT_EQ(1, ret);	
	EXPECT_EQ(3, seq.get());	
}

TEST_F(AtomicIntegerTest, addAndGet){
    int32_t ret = seq.addAndGet(2);
	EXPECT_EQ(3, ret);	
	EXPECT_EQ(3, seq.get());	
}

TEST_F(AtomicIntegerTest, incrementAndGet){
    int32_t ret = seq.incrementAndGet();
	EXPECT_EQ(2, ret);	
	EXPECT_EQ(2, seq.get());	
}

TEST_F(AtomicIntegerTest, decrementAndGet){
    int32_t ret = seq.decrementAndGet();
	EXPECT_EQ(0, ret);	
	EXPECT_EQ(0, seq.get());	
}

TEST_F(AtomicIntegerTest, add){
    seq.add(2);
	EXPECT_EQ(3, seq.get());	
}

TEST_F(AtomicIntegerTest, increment){
    seq.increment();
	EXPECT_EQ(2, seq.get());	
}

TEST_F(AtomicIntegerTest, decrement){
    seq.decrement();
	EXPECT_EQ(0, seq.get());	
}

TEST_F(AtomicIntegerTest, getAndSet){
    int32_t ret = seq.getAndSet(3);
	EXPECT_EQ(1, ret);	
	EXPECT_EQ(3, seq.get());		
}



