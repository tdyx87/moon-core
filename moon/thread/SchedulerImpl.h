#ifndef MOON_THREAD_SCHEDULERIMPL_H_
#define MOON_THREAD_SCHEDULERIMPL_H_

#include <moon/thread/Scheduler.h>

#include <boost/shared_ptr.hpp>

#include <map>
#include <vector>

namespace moon
{
class EventLoop;
class EventLoopThread;
class Worker;

class SchedulerImpl : public Scheduler
{
	typedef boost::shared_ptr<Worker> WorkerPtr;
public:
	SchedulerImpl(const std::string &name);
	virtual ~SchedulerImpl();
	
	virtual void start();
	virtual void shutdown(bool waitForJobsDone = true);

	virtual void submit(const Functor &cb);
	virtual void submitAt(const Functor &cb, const Timestamp &when);
	virtual void submitAfter(const Functor &cb, long delayMills);	

	virtual void submit(Runnable *runnable, bool autoRelease = true);
	virtual void submitAt(Runnable *runnable, bool autoRelease, const Timestamp &when);
	virtual void submitAfter(Runnable *runnable, bool autoRelease, long delayMills);	
	
	virtual void submit(ScheduleJob *job);
	virtual void submitAt(ScheduleJob *job, const Timestamp &when);
	virtual void submitAfter(ScheduleJob *job, long delayMills);	

private:
	void onDispatchJob(ScheduleJob *job);
	void onDispatchPendingJob(ScheduleJob *job, const Timestamp &when);	
	void onHandlePendingJob(ScheduleJob *job);

	void onJobFinished(ScheduleJob *job);
	void onJobFinishedHandleInLoop(ScheduleJob *job);
	
	void onShutdownInLoop(bool waitForJobsDone);
	
	WorkerPtr getNextWorker();
private:	
	std::vector<WorkerPtr> mWorkers;
	int mNext;

	EventLoop *mEventLoop;
	boost::shared_ptr<EventLoopThread> mEventLoopThread;
	
	std::map<ScheduleJob*, ScheduleJob*> mJobs;
	std::map< ScheduleJob*, ScheduleJob*> mPendingJobs;

	long mCompletedJobs;
};


}

#endif
