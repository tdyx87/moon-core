#include <moon/thread/Thread.h>
#include <moon/thread/CurrentThread.h>

#include <stdio.h>

using namespace moon;

namespace moon
{
namespace CurrentThread
{
__thread int t_cachedTid = 0;      // 当前线程ID
__thread char t_tidString[32];     // 当前线程ID字符串表示
__thread const char* t_threadName = "main";   // 线程名称

}  //~ CurrentThread
}  //~ moon

AtomicInt64 Thread::sNumCreated(0);  // 已创建线程的数量 

Thread::Thread(const ThreadFunc& func, const std::string& threadName)
: mThreadFunc(func), mName(createName(threadName))
{
	mState = ThreadState::NEW;
	mJoined = false;
}

Thread::~Thread()
{
}

/**
* 功能描述: 启动线程
* 参数: 无
* 返回值: 无
*/
void Thread::start()
{
	mState = ThreadState::RUNNING;  // 设置线程状态为"运行"状态
	// 启动线程
	if (pthread_create(&mPthreadId, NULL, startThread, static_cast<void *>(this)) != 0)
	{
		mState = ThreadState::ABORTED;  // 设置线程状态为"异常退出"状态
	}
}

/** 阻塞等待线程结束 */
void Thread::join()
{
	if (mJoined)
	{
		return ;
	}
    
	mJoined = true;
	pthread_join(mPthreadId, NULL);
}

// 线程启动函数
void* Thread::startThread(void *arg)
{
	Thread *pobjThread = static_cast<Thread *>(arg);

	pobjThread->mTid = CurrentThread::tid();                  // 获取线程tid
	CurrentThread::t_threadName = pobjThread->mName.c_str();  // 设置线程名称
	
	pobjThread->mThreadFunc(); 
	pobjThread->mState = ThreadState::FINISHED;  // 设置线程状态为"完成"状态

	return NULL;
}

std::string Thread::createName(const std::string &name)
{
    if (!name.empty()) {
		return name;
    }	
	
	char szBuf[32] = {0};
	unsigned long int num = reinterpret_cast<long int>(sNumCreated.incrementAndGet());
	snprintf(szBuf, sizeof(szBuf), "Thread-%lu", num);
	return std::string(szBuf);
}

