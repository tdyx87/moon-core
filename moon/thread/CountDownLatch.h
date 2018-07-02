/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_THREAD_COUNTDOWNLATHCC_H_
#define MOON_THREAD_COUNTDOWNLATHCC_H_

#include <moon/thread/Condition.h>
#include <moon/thread/MutexLock.h>

#include <boost/noncopyable.hpp>

namespace moon
{
class CountDownLatch : public boost::noncopyable
{
public:
	explicit CountDownLatch(int count);

	void wait();

	void countDown();

	int getCount() const;
private:
	mutable MutexLock mMutex;
	Condition mCondition;
	int mCount;
};

}  //~ namespace moon
#endif //~ 