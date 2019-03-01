#include <moon/core/SchedulerImpl.h>

#include <moon/Logger.h>
#include <moon/core/EventLoop.h>
#include <moon/core/EventLoopThread.h>
#include <moon/core/ScheduleJob.h>
#include <moon/core/Worker.h>

#include <moon/TypeCast.h>
#include <moon/Timestamp.h>
#include <moon/Number.h>

#include <boost/bind.hpp>

#include <assert.h>

using namespace moon;

static std::string getWorkName(int num) {
	std::string sequence = Number::intToString(num);
	return "Worker-" + sequence;
}

SchedulerPtr Scheduler::getDefaultScheduler(const std::string &name)
{
    return SchedulerPtr(new SchedulerImpl(name));
}

SchedulerImpl::SchedulerImpl(const std::string &name) 
  :	mEventLoopThread(new EventLoopThread(name))
{
    mNext = 0;
	mEventLoop = NULL; 
	mCompletedJobs = 0;
}

SchedulerImpl::~SchedulerImpl()
{ 
	assert(NULL != mEventLoop);
	shutdown(false);
}
	
void SchedulerImpl::start()
{
    mEventLoop = mEventLoopThread->startLoop();
	assert(NULL != mEventLoop);
	
	for (int i = 0; i < 5; ++i) {
		Worker *worker = new Worker(getWorkName(i), boost::bind(&SchedulerImpl::onJobFinished, this, _1));
		worker->start();
	    mWorkers.push_back(WorkerPtr(worker) );
	}
}

void SchedulerImpl::shutdown(bool waitForJobsDone)
{
	mEventLoop->runInLoop(boost::bind(&SchedulerImpl::onShutdownInLoop, this, waitForJobsDone));
}

void SchedulerImpl::submit(const Functor &cb)
{
    ScheduleJob *job = ScheduleJob::newJob(cb);
	submit(job);
}

void SchedulerImpl::submitAt(const Functor &cb, const Timestamp &when)
{
    ScheduleJob *job = ScheduleJob::newJob(cb);
	submitAt(job, when);
}

void SchedulerImpl::submitAfter(const Functor &cb, long delayMills)
{
    ScheduleJob *job = ScheduleJob::newJob(cb);
	submitAfter(job, delayMills);
}


void SchedulerImpl::submit(Runnable *runnable, bool autoRelease)
{
	ScheduleJob *job = ScheduleJob::newJob(runnable, autoRelease);
    submit(job);
}

void SchedulerImpl::submitAt(Runnable *runnable, bool autoRelease, const Timestamp &when)
{   
	ScheduleJob *job = ScheduleJob::newJob(runnable, autoRelease);
	submitAt(job, when);
}

void SchedulerImpl::submitAfter(Runnable *runnable, bool autoRelease, long delayMills)
{
	ScheduleJob *job = ScheduleJob::newJob(runnable, autoRelease);
	submitAfter(job, delayMills);
}

void SchedulerImpl::submit(ScheduleJob *job)
{
	mEventLoop->runInLoop(std::bind(&SchedulerImpl::onDispatchJob, this, job));
}

void SchedulerImpl::submitAt(ScheduleJob *job, const Timestamp &when)
{
	mEventLoop->runInLoop(std::bind(&SchedulerImpl::onDispatchPendingJob, this, job, when));
}

void SchedulerImpl::submitAfter(ScheduleJob *job, long delayMills)
{   
	Timestamp when = Timestamp::addMsecs(Timestamp::now(), delayMills);	
	mEventLoop->runInLoop(std::bind(&SchedulerImpl::onDispatchPendingJob, this, job, when));
}

void SchedulerImpl::onDispatchJob(ScheduleJob *job)
{
	assert(implicit_cast<size_t>(mNext) < mWorkers.size());
	
	WorkerPtr worker = mWorkers[mNext];
	++mNext;
	if (implicit_cast<size_t>(mNext) >= mWorkers.size()) {
		mNext = 0;
	}

	mJobs.insert(std::make_pair(job, job));
	worker->submit(job);
}

void SchedulerImpl::onDispatchPendingJob(ScheduleJob *job, const Timestamp &when)
{
    mPendingJobs.insert(std::make_pair(job, job));
	mEventLoop->runAt(std::bind(&SchedulerImpl::onHandlePendingJob, this, job), when);
}

void SchedulerImpl::onHandlePendingJob(ScheduleJob *job)
{
	std::map<ScheduleJob*, ScheduleJob*>::iterator it = mPendingJobs.find(job);   
	if (it == mPendingJobs.end()) {
		LOGGER_WARN("ScheduleJob[") << job << "] does not exist, maybe has cancelled.";
		return ;
	}

	assert(job == it->second);
	mPendingJobs.erase(it);	

	assert(NULL != job);
    onDispatchJob(job);
}

void SchedulerImpl::onJobFinished(ScheduleJob *job)
{
    mEventLoop->runInLoop(std::bind(&SchedulerImpl::onJobFinishedHandleInLoop, this, job));
}

void SchedulerImpl::onJobFinishedHandleInLoop(ScheduleJob *job)
{    
    LOGGER_TRACE("SchedulerImpl::onJobFinishedHandleInLoop");
	
    assert(NULL != job);

    std::map<ScheduleJob*, ScheduleJob*>::iterator it = mJobs.find(job);   
	assert(it != mJobs.end());
	mJobs.erase(it);	

    delete job;
	++mCompletedJobs;
}
	
void SchedulerImpl::onShutdownInLoop(bool waitForJobsDone)
{
    LOGGER_DEBUG("Scheduler going to shutdown, jobs[") << mJobs.size() 
		<< "], PendingJobs[" << mPendingJobs.size() << "], completed[" << mCompletedJobs << "].";

	if (!waitForJobsDone) {
		mJobs.clear();
	    mPendingJobs.clear();
	}
	
	for (size_t i = 0; i < mWorkers.size(); ++i) {
		LOGGER_DEBUG("Worker[%d] shutdown...", i);
		mWorkers[i]->shutdown();
	}
	mWorkers.clear();
}

SchedulerImpl::WorkerPtr SchedulerImpl::getNextWorker()
{
    assert(implicit_cast<size_t>(mNext) < mWorkers.size());
	
    WorkerPtr worker = mWorkers[mNext];
	++mNext;
	if (implicit_cast<size_t>(mNext) >= mWorkers.size()) {
		mNext = 0;
	}

	return worker;
}