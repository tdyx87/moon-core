#include <moon/thread/CurrentThread.h>

#include <time.h>

using namespace moon;

/** 判断是否是主线程 */
bool CurrentThread::isMainThread()
{
	return tid() == ::getpid();
}

/** 睡眠指定时间(单位:微秒) */
void CurrentThread::sleepUsec(long usec)
{
	static const int kMicroSecondsPerSecond = 1000 * 1000;
    struct timespec ts = { 0, 0 };
    ts.tv_sec = static_cast<time_t>(usec / kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(usec % kMicroSecondsPerSecond * 1000);
    ::nanosleep(&ts, NULL);
}



