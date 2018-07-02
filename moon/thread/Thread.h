/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_THREAD_THREAD_H_
#define MOON_THREAD_THREAD_H_

/**
* 文件名称: Thread.h
* 文件描述: 封装线程操作
*/

#include <moon/Thread/ThreadState.h>
#include <moon/AtomicInteger.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <pthread.h>
#include <stddef.h>
#include <string>

namespace moon
{

class Thread : boost::noncopyable
{
	typedef boost::function<void ()> ThreadFunc;
public:    
	/** 构造函数 */
	explicit Thread(const ThreadFunc& func, const std::string& threadName = std::string() );

	/** 析构函数 */
    ~Thread();


	/** 启动线程 */
	void start();

	/** 阻塞等待线程结束 */
	void join();
    
	/** 获取线程名称 */
	const std::string& name()const {return mName;}

	/** 获取线程标识ID */
	pid_t tid()const {return mTid;}

	bool started() const { return  ThreadState::RUNNING == mState; }

	/** 获取线程状态 */
	ThreadState::ThreadState state() const {return mState;}

	/** 判断线程是否正在运行 */
	bool isAlive()const {return mState == ThreadState::RUNNING;}
private:
	// 设置线程默认名称
    static std::string createName(const std::string &name);

	// 线程启动函数
	static void* startThread(void *arg); 
private:
	ThreadFunc mThreadFunc;      // 线程运行函数对象  	
    std::string mName;           // 线程名称

	pthread_t  mPthreadId;       // 线程ID
	pid_t mTid;                 // 线程标识ID(区别于线程ID:m_pthreadId_)
	
	ThreadState::ThreadState mState;  // 线程状态	
	bool mJoined;
	static AtomicInt64 sNumCreated; 
	
};  //~ class Thread



}  //~ namespace moon
#endif  //~ 