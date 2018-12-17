/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_THREAD_THREADJOB_H_
#define MOON_THREAD_THREADJOB_H_

namespace moon
{

class Runnable;

class Target
{
public:
	Target(Runnable *runnable, bool autoRelease)
		: mRunnable(runnable), mAutoRelease(autoRelease)
	{
	}

	Runnable* runnable() const {return mRunnable;}
	
	bool autoRelease() const {return mAutoRelease;}
private:
	Runnable *mRunnable;
	bool mAutoRelease;
};


}  // ~moon

#endif