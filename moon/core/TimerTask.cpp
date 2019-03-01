/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/core/TimerTask.h>

using namespace moon;

TimerTask::TimerTask(const TimerCallback& cb, Timestamp when, int count)
        : mCallback(cb), mExpiration(when), mCount(count), mPeriod(0)
{ 
	mSequence = 0;
	mExecutions = 0;
}
	
TimerTask::TimerTask(const TimerCallback& cb, Timestamp when, long mPeriodMsec, int count)
    : mCallback(cb), mExpiration(when),  mCount(count), mPeriod(mPeriodMsec)
{ 
	mSequence = 0;
	mExecutions = 0;
}
		

TimerTask::~TimerTask()
{
}

void TimerTask::reset(const Timestamp &now)
{
	mExpiration = repeat() ? Timestamp::addMsecs(now, mPeriod) : Timestamp::invalid();	
}



