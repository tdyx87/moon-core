#include <moon/thread/AbstractThreadPool.h>
#include <moon/thread/ThreadJob.h>
#include <moon/Runnable.h>

namespace moon{

AbstractThreadPool::AbstractThreadPool()
{
}

AbstractThreadPool::~AbstractThreadPool()
{
}

void AbstractThreadPool::purge()
{
 /*   for (ThreadJobList::iterator p = mJobs.begin(); p != mJobs.end(); ++p) {
        ThreadJob *job = *p;
		if (job->autoRelease()) {
			delete job->runnable();
		}
		delete job;
    }
    
	mJobs.clear();
	*/
}

}  // ~moon