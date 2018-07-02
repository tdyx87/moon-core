#include <moon/logger/LoggingPolicy.h>
#include <moon/thread/Condition.h>
#include <moon/thread/CountDownLatch.h>
#include <moon/thread/MutexLock.h>
#include <moon/thread/Thread.h>

#include <stdio.h>
#include <string>
#include <vector>

namespace moon
{

static const int kMaxProduceNum = 100000;
static const int mAlarmProduceNum = 1000;

class AsyncLoggingPolicy : public LoggingPolicy
{
public:
	void logging(const LoggerBufferPtr &buffer);
private:
	std::vector<LoggerBufferPtr> mProduceBuffer;
	std::vector<LoggerBufferPtr> mConsumeBuffer;

	bool mRunning;	
	CountDownLatch mLatch;
	MutexLock mMutex;
	Condition mCond;
	Thread mthread;
};

void AsyncLoggingPolicy::logging(const LoggerBufferPtr &buffer)
{
    MutexLockGuard lock(mMutex);
	mProduceBuffer.push_back(buffer);
}

}  // ~moon


