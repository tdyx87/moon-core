#include <moon/core/Worker.h>

#include <moon/Logger.h>
#include <moon/core/ScheduleJob.h>
#include <moon/core/EventLoop.h>
#include <moon/core/EventLoopThread.h>

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
	mEventLoop->runInLoop(std::bind(&Worker::onHandleJob, this, job));
}

void Worker::onHandleJob(ScheduleJob *job)
{
	assert(job != NULL);
    Runnable *runnable = job->runnable();
    if (NULL != runnable) {
		runnable->run();
		if (job->autoRelease()) {
			delete runnable;
			runnable = NULL;
		}
    } else if (job->callback()) {
	    job->callback()();
	} else {
	    LOGGER_WARN("Unknown job, can not handle this job.");
	}

	if (!mJobCompletedListener) {
		delete job;
		return ;
	}

	mJobCompletedListener(job);
}




}  // ~moon

