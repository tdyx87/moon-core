#include <moon/thread/Condition.h>

#include <time.h>

using namespace moon;

bool Condition::waitForSeconds(int seconds)
{
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);
	abstime.tv_sec += seconds;
	return 0 == pthread_cond_timedwait(&mCond, mMutex.getPthreadMutex(), &abstime);
}


