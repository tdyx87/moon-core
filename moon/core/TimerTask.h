/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou

  一个TimerTask对象表示一个定时任务。
  每一个TimerTask对象有两个重要属性：
  1 执行时间点：通过TimerTask::expiration()方法获取。
  2 任务执行间隔：通过TimerTask::repeat()判断该任务是否需要循环执行。

*/

#ifndef MOON_CORE_TIMERTASK_H_
#define MOON_CORE_TIMERTASK_H_

#include <moon/Timestamp.h>
#include <moon/core/TimerCallback.h>

#include <stdint.h>

namespace moon 
{

class TimerTask
{
public:
	TimerTask(const TimerCallback& cb, Timestamp when, int count = 0);
		
    TimerTask(const TimerCallback& cb, Timestamp when, long mPeriodMsec, int count = 0);

	~TimerTask();

	void run()  
    {
		if (mCallback) {
		    ++mExecutions;
			mCallback();
		}
	}

    Timestamp expiration() const     { return mExpiration; }

	// 返回true表示该定时任务需要重复执行，执行间隔时间由mPeriod成员值指定
	bool repeat() const { return (mPeriod > 0) && ( (0 == mCount) || (mExecutions < mCount) ) ; }	

	// 重置任务运行时间，任务下一次执行时间为@now指定的值加上由mPeriod成员变量指定的值。
	// 注意：只有在repeat()返回true的时候调用该方法才有效果，否则将重置一个无效的定时时间。
	void reset(const Timestamp &now);

	uint64_t sequence() const {return mSequence;}	
	void setSequence(uint64_t sequence) {mSequence = sequence;}
private:
	const TimerCallback mCallback;
	Timestamp mExpiration;
	const int mCount;
	const long mPeriod;		
	uint64_t mSequence;
	int mExecutions;
};

}  //~ moon

#endif