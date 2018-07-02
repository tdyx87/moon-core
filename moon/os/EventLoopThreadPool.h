/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_OS_EVENTLOOPTHREADPOOL_H_
#define MOON_OS_EVENTLOOPTHREADPOOL_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <vector>

namespace moon
{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : public boost::noncopyable
{
	typedef boost::function<void(EventLoop*)> ThreadInitCallback;
public:
	EventLoopThreadPool(EventLoop* baseLoop);
    ~EventLoopThreadPool();

	void setThreadNum(int numThreads) { mNumThreads = numThreads; }
    void start(const ThreadInitCallback& cb = ThreadInitCallback());

    /** valid after calling start(), round-robin */
    EventLoop* getNextLoop();

	bool started() const { return mStarted; }

private:
	EventLoop* mBaseLoop;
	bool mStarted;
	int mNumThreads;
	int mNext;
	boost::ptr_vector<EventLoopThread> mThreads;
	std::vector<EventLoop*> mEventLoops;
};




}  // ~moon
#endif  // ~