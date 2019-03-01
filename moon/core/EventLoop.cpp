/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/Logger.h>
#include <moon/core/EventLoop.h>
#include <moon/core/EventChannel.h>
#include <moon/core/Poller.h>
#include <moon/core/Timer.h>

#include <sys/eventfd.h>
#include <assert.h>

namespace moon
{

static const int kPollTimeMs = 60 * 1000;

__thread EventLoop* t_loopInThisThread = NULL;

static int createEventfd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0) {
	    LOGGER_FATAL("Failed in eventfd");
	    abort();
	}
	return evtfd;
}

EventLoop::EventLoop() 
	: mThreadId(CurrentThread::tid()),
	  mPoller(Poller::newDefaultPoller(this)),
	  mTimer(new Timer(this)),
	  mWakeupFd(createEventfd()),
	  mWakeupChannel(new EventChannel(mWakeupFd, this))
{
	if (t_loopInThisThread != NULL) {
		LOGGER_FATAL("Another EventLoop exists in this thread");
	} else {		
		t_loopInThisThread = this;
		mIsQuit = false;
	    mIsLooping = false;
		mIsEventHandling = false;
		mIsCallingPendingFunctors = false;
		mCurrentActiveChannel = NULL;

		mWakeupChannel->setReadCallback(std::bind(&EventLoop::handleRead, this));
		mWakeupChannel->enableReading();
	}	
}

EventLoop::~EventLoop()
{
	mWakeupChannel->remove();
	::close(mWakeupFd);
	t_loopInThisThread = NULL;
}

/** 获取当前线程的CEventLoop */
EventLoop* EventLoop::currentEventLoop()
{
	return t_loopInThisThread;
}

/** 循环监听事件 */
void EventLoop::loop()
{
	assert(!mIsLooping);
    this->assertInLoopThread();

	mIsLooping = true;	
    mIsQuit = false;
	int iErrorNum = 0;

	while (!mIsQuit) {
		mEventChannelList.clear();
        
		int num = mPoller->poll(kPollTimeMs, &mEventChannelList);
		if (num == 0) {
			iErrorNum = 0;
			continue;
		} else if (num < 0) {
			++iErrorNum;
			assert(iErrorNum < 10);			
		}
		iErrorNum = 0;
        
		mIsEventHandling = true;
		for (EventChannelList::iterator it = mEventChannelList.begin(); it != mEventChannelList.end(); ++it) {
			mCurrentActiveChannel = *it;
			mCurrentActiveChannel->handleEvent();
		}

		mCurrentActiveChannel = NULL;
		mIsEventHandling = false;
		this->doPendingFunctors();

		this->mDelayedDeletes.clear();
	}	

	LOGGER_TRACE("EventLoop stop looping");
	mIsLooping = false;
}

/** 更新I/O事件 */
void EventLoop::updateEventChannel(EventChannel* pEventChannel)
{	
	assert(pEventChannel->ownerLoop() == this);
	assertInLoopThread();

	mPoller->updateEventChannel(pEventChannel);
}


/** 移除指定的事件通道 */
void EventLoop::removeEventChannel(EventChannel* pEventChannel)
{
	assert(pEventChannel->ownerLoop() == this);
	assertInLoopThread();

	mPoller->removeEventChannel(pEventChannel);
}

void EventLoop::quit()
{
    if (mIsQuit) {
		return ;
    }
	mIsQuit = true;
	if (!isInLoopThread()) {
	    wakeup();
	}
}

void EventLoop::runInLoop(const Functor& cb)
{
	if (this->isInLoopThread()) {
	    cb();
	} else {
	    this->queueInLoop(cb);
	}
}

void EventLoop::queueInLoop(const Functor& cb)
{
	{
	    MutexLockGuard lock(mMutexLock);
	    mPendingFunctors.push_back(cb);
	}

	if ( (!this->isInLoopThread()) || mIsCallingPendingFunctors) {
	    this->wakeup();
	}
}

TimerTaskId EventLoop::runAt(const TimerCallback &cb, const Timestamp &when, long periodMills, int count)
{
    return mTimer->add(cb, when, periodMills, count);
}

TimerTaskId EventLoop::runAfter(const TimerCallback &cb, long delayMillis, long periodMills, int count)
{
    Timestamp when = Timestamp::addMsecs(Timestamp::now(), delayMillis);	
    return mTimer->add(cb, when, periodMills, count);
}

void EventLoop::cancel(const TimerTaskId &timerTaskId)
{
    mTimer->cancel(timerTaskId);
}

void EventLoop::abortNotInLoopThread()
{
	LOGGER_FATAL("EventLoop::abortNotInLoopThread - this EventLoop was created in threadId = %d, current thread id = %d", mThreadId, CurrentThread::tid());
	assert(false);
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = ::write(mWakeupFd, &one, sizeof one);
	if (n != sizeof one) {
		LOGGER_ERROR("EventLoop::wakeup() writes %ld bytes instead of 8", n);
	}
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	ssize_t n = ::read(mWakeupFd, &one, sizeof one);
	if (n != sizeof one) {
		LOGGER_ERROR("EventLoop::handleRead() reads %ld bytes instead of 8", n);
	}
}

void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	mIsCallingPendingFunctors = true;

	{
	    MutexLockGuard lock(mMutexLock);
	    functors.swap(mPendingFunctors);
	}

	for (size_t i = 0; i < functors.size(); ++i) {
	    functors[i]();
	}
	mIsCallingPendingFunctors = false;
}


}  // ~moon