#include <moon/thread/Worker.h>
#include <moon/thread/ScheduleJob.h>
#include <moon/os/EventLoop.h>
#include <moon/os/EventLoopThread.h>
#include <moon/logger/Logger.h>

#include <boost/bind.hpp>
#include <assert.h>

namespace moon{

AtomicInt32 Worker::sSequence = 0;

Worker::Worker(const std::string &name, const OnJobCompletedListener &listener)
    : mName(name),
	  mEventLoopThread(new EventLoopThread(name)),
	  mJobCompletedListener(listener)
{
    mEventLoop = NULL;
}

Worker::~Worker()
{
    assert(NULL != mEventLoop);
    LOGGER_TRACE("Worker::~Worker [%s]", mName.c_str());
}

void Worker::start()
{	
	mEventLoop = mEventLoopThread->startLoop();
}

void Worker::shutdown()
{
    mEventLoop->quit();
}

void Worker::submit(ScheduleJob *job)
{
	mEventLoop->runInLoop(boost::bind(&Worker::onHandleJob, this, job));
}

void Worker::onHandleJob(ScheduleJob *job)
{
    Runnable *runnable = job->runnable();
    if (NULL != runnable)
	{
		runnable->run();
		if (job->autoRelease()) 
		{
			delete runnable;
			runnable = NULL;
		}
    }
	else if (job->callback()) 
	{
	    job->callback()();
	}
	else
	{
	    LOGGER_WARN("Unknown job, can not handle this job.");
	}

	if (!mJobCompletedListener) {
		delete job;
		return ;
	}

	mJobCompletedListener(job);
}




}  // ~moon

