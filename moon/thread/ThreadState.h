/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_THREAD_THREADSTATE_H_
#define MOON_THREAD_THREADSTATE_H_

/**
* 文件名称: ThreadState.h
* 文件描述: 描述线程某一时刻的状态,在一个给的的之间只能处于一种状态
*/

namespace moon
{
namespace ThreadState
{

enum ThreadState
	{
	    NEW = 100,        // 线程处于刚"创建"状态
	    RUNNING = 110,    // 线程处于"运行"状态
	    FINISHED = 120,   // 线程处于"完成"状态
	    ABORTED = 130     // 线程处于"异常退出"状态
    };


}  // ~namespace ThreadState
}  // ~namespace moon

#endif  // ~