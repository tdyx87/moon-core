/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_OS_EVENTLOOP_H_
#define MOON_OS_EVENTLOOP_H_

#include <moon/Functor.h>
#include <moon/noncopyable.h>
#include <moon/Timestamp.h>
#include <moon/os/TimerCallback.h>
#include <moon/os/TimerTaskId.h>
#include <moon/thread/CurrentThread.h>
#include <moon/thread/MutexLock.h>

#include <memory>
#include <vector>

namespace moon
{
class Poller;
class EventChannel;
class Timer;

class EventLoop : noncopyable
{
public:
	EventLoop();
    ~EventLoop();
    
	/** 获取当前线程的EventLoop指针 */
	static EventLoop* currentEventLoop();

	void loop();
    
	void quit();

	/** 
	  Runs callback immediately in the loop thread.It wakes up the loop, and run the cb.
	  If in the same loop thread, cb is run within the function.Safe to call from other threads.
	*/
	void runInLoop(const Functor& cb);

	/** 
	    Queues callback in the loop thread.Runs after finish pooling.Safe to call from other threads.
	*/
	void queueInLoop(const Functor& cb);

	/**
	  在@when指定的时间执行@cb指定的函数对象。	  该方法线程安全，可从其它线程调用该方法。
	  1 如果periodMills为0，cb引用的函数对象只会运行一次。如果periodMills为0，count不起任何作用。
	  2 如果periodMills不为0，第一次执行以后每隔periodMills毫秒都会调用一次cb引用的函数对象。
	    如果count大于0，则表示cb调用count次以后就会自动停止，如果count小于等于0，则不起任何作用。
	  
	  如果@periodMills大于0表示到达@when指定的时间后，每隔@periodMills执行一次@cb指定的函数对象。
	*/
	TimerTaskId runAt(const TimerCallback &cb, const Timestamp &when, long periodMills = 0L, int count = 0);

	/**
	  在当前时间过去@delayMillis毫秒后执行@cb函数对象。	  该方法线程安全，可从其它线程调用该方法。
	  1 如果periodMills为0，cb引用的函数对象只会运行一次。如果periodMills为0，count不起任何作用。
	  2 如果periodMills不为0，第一次执行以后每隔periodMills毫秒都会调用一次cb引用的函数对象。
	    如果count大于0，则表示cb调用count次以后就会自动停止，如果count小于等于0，则不起任何作用。
	  
	  如果@periodMills大于0表示到达@when指定的时间后，每隔@periodMills执行一次@cb指定的函数对象。
	*/
	TimerTaskId runAfter(const TimerCallback &cb, long delayMillis, long periodMills = 0L, int count = 0);

	// 取消@timerTaskId对应的定时任务。
	void cancel(const TimerTaskId &timerTaskId);
	
	void assertInLoopThread()
	{
		if (!isInLoopThread()) {
			abortNotInLoopThread();
		}
	}
     
	bool isInLoopThread() const { return mThreadId == moon::CurrentThread::tid(); }

	const Poller& getPoller()  {return *mPoller.get();}

	void deleteLater(const std::shared_ptr<void> &obj) {mDelayedDeletes.push_back(obj);}
private:
	/** 更新I/O事件 */
	void updateEventChannel(EventChannel* pEventChannel);

	/** 移除指定的事件通道 */
	void removeEventChannel(EventChannel* pEventChannel);

	void abortNotInLoopThread();

	void wakeup(); 
	void handleRead();  
    void doPendingFunctors();
		
private:
	friend class EventChannel;
	typedef std::vector<EventChannel *> EventChannelList;	

    bool mIsLooping;
	bool mIsQuit;	
	bool mIsEventHandling; 
    bool mIsCallingPendingFunctors; 
	EventChannel* mCurrentActiveChannel;

	const pid_t mThreadId;
	EventChannelList mEventChannelList;
	std::unique_ptr<Poller> mPoller;
	std::unique_ptr<Timer> mTimer;

	int mWakeupFd;
	std::unique_ptr<EventChannel> mWakeupChannel;
    
	MutexLock mMutexLock;
	std::vector<Functor> mPendingFunctors; 
	std::vector<std::shared_ptr<void>> mDelayedDeletes;
};


}  // ~moon

#endif  // ~