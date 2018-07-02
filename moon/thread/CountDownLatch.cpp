#include <moon/thread/CountDownLatch.h>

using namespace moon;

CountDownLatch::CountDownLatch(int count) : mMutex(), mCondition(mMutex), mCount(count)
{
}

void CountDownLatch::wait()
{
	MutexLockGuard lock(mMutex);
	while (mCount > 0)
	{
	    mCondition.wait();
	}
}

void CountDownLatch::countDown()
{
	MutexLockGuard lock(mMutex);
	--mCount;
	if (mCount == 0)
	{
	    mCondition.notifyAll();
	}
}

int CountDownLatch::getCount() const
{
	MutexLockGuard lock(mMutex);
    return mCount;
}
