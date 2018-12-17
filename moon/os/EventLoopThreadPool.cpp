/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/os/EventLoopThreadPool.h>

#include <moon/TypeCast.h>
#include <moon/os/EventLoop.h>
#include <moon/os/EventLoopThread.h>

#include <assert.h>

using namespace moon;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
	: mBaseLoop(baseLoop),
	  mStarted(false),
	  mNumThreads(0),
	  mNext(0)
{
}

EventLoopThreadPool::~EventLoopThreadPool()
{
}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
	assert(!mStarted);
	mBaseLoop->assertInLoopThread();

	mStarted = true;

	for (int i = 0; i < mNumThreads; ++i) {
		EventLoopThread* t = new EventLoopThread("", cb);
        mThreads.push_back(std::unique_ptr<EventLoopThread>(t));
        mEventLoops.push_back(t->startLoop());
	}
    
	// if mNumThreads is 0, which means this server wanted to be single thread
	if ( (0 == mNumThreads) && cb) {
		cb(mBaseLoop);
	}
}

/** valid after calling start(), round-robin */
EventLoop* EventLoopThreadPool::getNextLoop()
{
	mBaseLoop->assertInLoopThread();
	assert(mStarted);

	EventLoop* loop = mBaseLoop;

	if (!mEventLoops.empty()) {
		// round-robin
		loop = mEventLoops[mNext];
		++mNext;
		if (implicit_cast<size_t>(mNext) >= mEventLoops.size()) {
		    mNext = 0;
		}
	}

	return loop;
}
