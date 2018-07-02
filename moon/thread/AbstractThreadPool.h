/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_THREAD_ABSTRACTTHREADPOOL_H_
#define MOON_THREAD_ABSTRACTTHREADPOOL_H_

#include <stddef.h>

#include <list>

namespace moon{

class Runnable;
class ThreadJob;

class AbstractThreadPool
{
public:
	virtual ~AbstractThreadPool();

	virtual bool submit(Runnable *runnable, bool autoRelease = true) = 0;
	virtual bool shutdown() = 0;
protected:
	typedef std::list<ThreadJob *> ThreadJobList;
	AbstractThreadPool();

	void purge();	
private:
	ThreadJobList mJobs;
};


}   // ~moon

#endif