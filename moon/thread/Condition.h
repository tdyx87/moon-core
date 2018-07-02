/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_THREAD_CONDITION_H_
#define MOON_THREAD_CONDITION_H_

/**
* 文件名称: Condition.h
* 文件描述: 封装线程条件变量
*/

#include <moon/thread/MutexLock.h>
#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace moon
{

class Condition : boost::noncopyable
{
public:
	// 构造函数 
    explicit Condition(MutexLock& mutex) : mMutex(mutex)
	{
		pthread_cond_init(&mCond, NULL);
	}

	// 析构函数 
	~Condition() {pthread_cond_destroy(&mCond);}

	// 等待 */
	void wait()
	{
		pthread_cond_wait(&mCond, mMutex.getPthreadMutex());
	}

    bool waitForSeconds(int seconds);

	// 唤醒一个线程 
	void notify() {pthread_cond_signal(&mCond);}

	// 唤醒所有线程 
	void notifyAll() {pthread_cond_signal(&mCond);}
private:
    MutexLock& mMutex;   
    pthread_cond_t mCond; 
};  // ~Condition



}  // ~namespace moon
#endif  // ~