/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_THREAD_SCHEDULER_H_
#define MOON_THREAD_SCHEDULER_H_

#include <moon/Functor.h>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace moon
{
class Runnable;
class ScheduleJob;
class Timestamp;
class Scheduler;

typedef boost::shared_ptr<Scheduler> SchedulerPtr;

class Scheduler
{
public:
	Scheduler();
	virtual ~Scheduler();
	
	virtual void start() = 0;
	virtual void shutdown(bool waitForJobsDone = true) = 0;

	virtual void submit(const Functor &cb) = 0;
	virtual void submitAt(const Functor &cb, const Timestamp &when) = 0;
	virtual void submitAfter(const Functor &cb, long delayMills) = 0;	

	virtual void submit(Runnable *runnable, bool autoRelease = true) = 0;
	virtual void submitAt(Runnable *runnable, bool autoRelease, const Timestamp &when) = 0;
	virtual void submitAfter(Runnable *runnable, bool autoRelease, long delayMills) = 0;	
	
	virtual void submit(ScheduleJob *job) = 0;
	virtual void submitAt(ScheduleJob *job, const Timestamp &when) = 0;
	virtual void submitAfter(ScheduleJob *job, long delayMills) = 0;	

	static SchedulerPtr getDefaultScheduler(const std::string &name);
	
};


}

#endif
