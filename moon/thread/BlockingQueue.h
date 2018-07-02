/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_THREAD_BLOCKINGQUEUE_H_
#define MOON_THREAD_BLOCKINGQUEUE_H_

#include <moon/thread/Condition.h>
#include <moon/thread/MutexLock.h>

#include <boost/noncopyable.hpp>

#include <algorithm>
#include <deque>

namespace moon
{

template<class T>
class BlockingQueue : public boost::noncopyable
{
public:
	BlockingQueue() : mMutex(), mNotEmpty(mMutex), mQueue() {}

    void put(const T& x)
	{
		MutexLockGuard lock(mMutex);
		mQueue.push_back(x);
        mNotEmpty.notify(); // wait morphing saves us
	}

	T take()
	{
		MutexLockGuard lock(mMutex);
		while(mQueue.empty())
		{
			mNotEmpty.wait();
		}
		
		assert(!mQueue.empty());

		T front = mQueue.front();
        mQueue.pop_front();

		return front;
	}

	size_t size() const
	{
		MutexLockGuard lock(mMutex);
		return mQueue.size();
	}

	bool contains(const T& x)
	{
		return std::find(mQueue.begin(), mQueue.end(), x) != mQueue.end();
	}
private:
	mutable MutexLock mMutex;
    Condition mNotEmpty;
	std::deque<T> mQueue;
};


}  // ~namespace moon

#endif  //  ~