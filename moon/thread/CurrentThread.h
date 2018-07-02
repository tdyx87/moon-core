/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_THREAD_CURRENTTHREAD_H_
#define MOON_THREAD_CURRENTTHREAD_H_

/**
* 文件名称: CurrentThread.h
* 文件描述: 
*/

#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>


namespace moon
{
namespace CurrentThread
{

extern __thread int t_cachedTid;           // 当前线程ID
extern __thread char t_tidString[32];      // 当前线程ID字符串表示
extern __thread const char* t_threadName;  // 线程名称


/** 获取线程ID */
inline pid_t tid()
{
	if (t_cachedTid == 0)
	{
		t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
		snprintf(t_tidString, sizeof(t_tidString), "%5d", t_cachedTid);
	}

	return t_cachedTid;
}

/** 获取线程ID字符串形式 */
inline const char* tidString()
{
	tid();
	return t_tidString;
}

/** 获取线程名称 */
inline const char* name()
{
	return t_threadName;
}

/** 判断是否是主线程 */
bool isMainThread();

/** 睡眠指定时间(单位:微秒) */
void sleepUsec(long usec);



}  //~ namespace CurrentThread
}  //~ namespace moon


#endif  //~ 