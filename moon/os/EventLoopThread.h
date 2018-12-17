/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_OS_EVENTLOOPTHREAD_H_
#define MOON_OS_EVENTLOOPTHREAD_H_

#include <moon/AtomicInteger.h>
#include <moon/noncopyable.h>
#include <moon/thread/CountDownLatch.h>
#include <moon/thread/MutexLock.h>
#include <moon/thread/Thread.h>

namespace moon
{

class EventLoop;

class EventLoopThread : public noncopyable
{
	typedef std::function<void(EventLoop*)> ThreadInitCallback;
public:
	EventLoopThread(const std::string &name, const ThreadInitCallback& cb = ThreadInitCallback());
    ~EventLoopThread();
    
    EventLoop* eventLoop() {return mEventLoop;}
	EventLoop* startLoop();

	void quit();

	std::string name() const {return mThread.name();}
private:
	void threadFunc();

	static std::string createName(const std::string &name);
private:	
	static AtomicInt64 sNumCreated; 	
	
	EventLoop *mEventLoop;
	Thread mThread;
	MutexLock mMutex;
	CountDownLatch mLatch;

	bool mStarted;
	ThreadInitCallback mThreadInitCallback;
	
};


}  // ~moon
#endif  // ~