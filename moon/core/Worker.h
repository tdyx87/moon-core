/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_OS_WORKER_H_
#define MOON_OS_WORKER_H_

#include <moon/AtomicInteger.h>
#include <moon/Functor.h>
#include <moon/thread/MutexLock.h>

#include <functional>
#include <memory>

namespace moon
{
class EventLoop;
class EventLoopThread;
class ScheduleJob;
class Timestamp;

class Worker
{
public:
	typedef std::function<void (ScheduleJob *) > OnJobCompletedListener;
	Worker(const std::string &name, const OnJobCompletedListener &listener = OnJobCompletedListener());
	~Worker();
	
	virtual void start();
	virtual void shutdown();
	
	virtual void submit(ScheduleJob *job);	
	std::string getName() const {return mName;}
private:    
    void onHandleJob(ScheduleJob *job);
private:
	static AtomicInt32 sSequence;
	std::string mName;
	
	EventLoop *mEventLoop;
	std::shared_ptr<EventLoopThread> mEventLoopThread;

	OnJobCompletedListener mJobCompletedListener;
};


}

#endif