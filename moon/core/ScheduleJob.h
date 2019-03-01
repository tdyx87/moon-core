/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_CORE_SCHEDULEJOB_H_
#define MOON_CORE_SCHEDULEJOB_H_

#include <moon/Functor.h>
#include <moon/noncopyable.h>
#include <moon/Runnable.h>

namespace moon {

class ScheduleJob : noncopyable
{
public:
    static ScheduleJob* newJob(Runnable *runnable, bool autoRelease = true) 
    {
		return new ScheduleJob(runnable, autoRelease);
    }

	static ScheduleJob* newJob(const Functor &cb) 
	{
		return new ScheduleJob(cb);
    }

	Runnable* runnable() const {return mRunnable;}
	bool autoRelease() const {return mAutoRelease;}
	
	Functor callback() const {return mCallback;}
private:	
	explicit ScheduleJob(Runnable *runnable, bool autoRelease = true) 
		: mRunnable(runnable), mAutoRelease(autoRelease)
	{
	}

	explicit ScheduleJob(const Functor &cb)
	    : mRunnable(NULL), mCallback(cb)
	{
	    mAutoRelease = false;
	}
private:
	Runnable *mRunnable;
	bool mAutoRelease;
	Functor mCallback;
	
};  // ~Job


}  // ~moon

#endif