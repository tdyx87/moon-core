/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/core/EventLoop.h>
#include <moon/core/EventLoopThread.h>

#include <assert.h>
#include <stdio.h>
#include <functional>

using namespace moon;

AtomicInt64 EventLoopThread::sNumCreated(0); 

EventLoopThread::EventLoopThread(const std::string &name, const ThreadInitCallback& cb)
	: mThread(std::bind(&EventLoopThread::threadFunc, this), "EventLoopThread"),
	  mMutex(),
      mLatch(1),
	  mThreadInitCallback(cb)
{
	mEventLoop = NULL;
	mStarted = false;
}

EventLoopThread::~EventLoopThread()
{
	quit();
}

EventLoop* EventLoopThread::startLoop()
{
    {
        MutexLockGuard guard(mMutex);
		if (mStarted) {
			assert(false);
			return NULL;
		}
		mStarted = true;
    }
	
	mThread.start();
	mLatch.wait();	

	return mEventLoop;
}

void EventLoopThread::quit()
{
    if (NULL == mEventLoop) {
        return ;
    }

	EventLoop *loop = NULL;
    {
        MutexLockGuard guard(mMutex);
		if (NULL == mEventLoop) {
            return ;
        }
		loop = mEventLoop;
		mEventLoop = NULL;
    }

	loop->quit();
	mThread.join();	  
}

void EventLoopThread::threadFunc()
{
	EventLoop loop;

	if (mThreadInitCallback) {
	    mThreadInitCallback(&loop);
	}
	
	mEventLoop = &loop;
	mLatch.countDown();
	loop.loop();
}

std::string EventLoopThread::createName(const std::string &name)
{
    if (!name.empty()) {
		return name;
    }	
	
	char szBuf[32] = {0};
	unsigned long int num = reinterpret_cast<long int>(sNumCreated.incrementAndGet());
	snprintf(szBuf, sizeof(szBuf), "EventLoopThread-%lu", num);
	return std::string(szBuf);
}

