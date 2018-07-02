/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_THREAD_MUTEXTLOCK_H_
#define MOON_THREAD_MUTEXTLOCK_H_

/**
* 文件名称: MutexLock.h
* 文件描述: 
*/

#include <pthread.h>
#include <boost/noncopyable.hpp>

namespace moon
{

/**
* 类名称: MutexLock
* 类描述: 封装线程互斥锁
*/
class MutexLock : boost::noncopyable
{
public:
	/** 构造函数 */
    MutexLock() {pthread_mutex_init(&mMutex, NULL);}

	/** 析构函数 */
	~MutexLock() {pthread_mutex_destroy(&mMutex);}

    /** 加锁 */
	void lock() {pthread_mutex_lock(&mMutex);}

	/** 解锁 */
	void unlock() {pthread_mutex_unlock(&mMutex);}

	/**
	* 函数描述: 获取指向线程互斥对象的指针,CCondition对象调用
	* 返回值: 指向线程互斥对象的指针
	*/
	pthread_mutex_t* getPthreadMutex() {return &mMutex;}
private:
	pthread_mutex_t mMutex;  // 线程锁对象
};  // ~MutexLock



/**
* 类名称: MutextLockGuard
* 类描述: 线程互斥锁使用对象,采用RAII机制
*/
class MutexLockGuard : boost::noncopyable
{
public:
	/** 构造函数,对象构造时加锁 */
	explicit MutexLockGuard(MutexLock& mutex) : mMutex(mutex) {mMutex.lock();}

	/** 析构函数,对象销毁时解锁 */
	~MutexLockGuard(){mMutex.unlock();}
  
private:
	MutexLock& mMutex;
};  // ~MutextLockGuard


#define MutexLockGuard(x) error "Missing guard object name"


}  // ~namespace moon

#endif // ~