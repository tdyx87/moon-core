/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_OS_TIMERTASKID_H_
#define MOON_OS_TIMERTASKID_H_

#include <moon/os/TimerTask.h>

namespace moon 
{
class TimerTaskId
{
public:
	TimerTaskId() {}
	TimerTaskId(TimerTask *timerTask, int64_t sequence)
        : mTimerTask(timerTask), mSequence(sequence)
    { 
	}

	int64_t id() const {return mSequence;}	
private:
    TimerTask* getTimerTask() const {return mTimerTask;}
private:
    friend class Timer;
	TimerTask *mTimerTask;
	int64_t mSequence;
};

}

#endif
