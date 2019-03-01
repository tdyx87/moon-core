/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/core/Timer.h>

#include <moon/Logger.h>
#include <moon/core/EventLoop.h>

#include <time.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include <assert.h>
#include <stdint.h>

using namespace moon;

static int createTimerfd()
{
	int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (timerfd < 0) {
		LOGGER_FATAL("Failed int timerfd_create");
	}

	return timerfd;
}

static void readTimerfd(int timefd, const Timestamp &now)
{
	uint64_t howmany = 0;
	ssize_t n = read(timefd, &howmany, sizeof(howmany));
	LOGGER_TRACE("readTimerfd howmany[%lu] at %s", howmany, now.toMicroFormattedString().c_str());

	if (n != sizeof(howmany)) {
		LOGGER_ERROR("readTimerfd reads %lu bytes instead of 8\n", howmany);
    }
}

static timespec howMuchTimeFromNow(const Timestamp &when)
{
	int64_t microSeconds = when.usecs() - Timestamp::now().usecs();
	if (microSeconds < 100) {
		microSeconds = 100;
	}

	struct timespec ts;
	ts.tv_sec = static_cast<time_t>(microSeconds / Timestamp::kMicroSecondsPerSecond);
	ts.tv_nsec = static_cast<time_t>( (microSeconds % Timestamp::kMicroSecondsPerSecond) * 1000);
	return ts;
}

static void resetTimerfd(int timerfd, const Timestamp &expiration)
{
	struct itimerspec newValue;
	bzero(&newValue, sizeof newValue);

	newValue.it_value = howMuchTimeFromNow(expiration);
	int ret = ::timerfd_settime(timerfd, 0, &newValue, NULL);
    if (ret) {
        LOGGER_ERROR("timerfd_settime()");
    }
}

AtomicInt64 Timer::sNumCreated = AtomicInt64(0);

Timer::Timer(EventLoop* loop) 
	: mEventLoop(loop), mTimerfd(createTimerfd()), mEventChannel(mTimerfd, loop)
{
	mCallingExpiredTimerTasks = false;
	mEventChannel.setReadCallback(std::bind(&Timer::handleRead, this));
	mEventChannel.enableReading();
}

Timer::~Timer()
{
	mEventChannel.remove();
	::close(mTimerfd);
}	

TimerTaskId Timer::add(const TimerCallback& cb, const Timestamp &when, long periodMsec, int count)
{
//	LOGGER_TRACE("Timer::add when:%s", when.toMicroFormattedString().c_str());

	assert(mEventLoop != NULL);
	
	const uint64_t sequence = sNumCreated.incrementAndGet();
	TimerTask* timerTask = new TimerTask(cb, when, periodMsec, count);
	
	timerTask->setSequence(sequence);
	mEventLoop->runInLoop(std::bind(&Timer::addTimerInLoop, this, timerTask));
	
	return TimerTaskId(timerTask, sequence);
}

void Timer::cancel(const TimerTaskId &timerTaskId)
{
	assert(mEventLoop != NULL);
	mEventLoop->runInLoop(std::bind(&Timer::cancelTimerInLoop, this, timerTaskId));
}

void Timer::addTimerInLoop(TimerTask *timerTask)
{
	mEventLoop->assertInLoopThread();

	bool earliestChanged = add(TimerTaskPtr(timerTask));
	
	if (earliestChanged) {
	    resetTimerfd(mTimerfd, timerTask->expiration());
	}	
}

void Timer::cancelTimerInLoop(const TimerTaskId &timerTaskId)
{
	mEventLoop->assertInLoopThread();
    TimerTaskList::iterator pos = find(timerTaskId.getTimerTask()->expiration().usecs(), timerTaskId.getTimerTask());
    if (mTimerTasks.end() != pos) {
		mTimerTasks.erase(pos);
    } else if (mCallingExpiredTimerTasks) {
		mCancelingTimerTasks.insert(timerTaskId.getTimerTask());
	}	
}

void Timer::handleRead()
{
	mEventLoop->assertInLoopThread();	
    Timestamp now(Timestamp::now());
    readTimerfd(mTimerfd, now);

	std::vector<TimerTaskPtr> expiredTimerTasks;	
	const size_t count = getExpired(now, expiredTimerTasks);
	assert(count == expiredTimerTasks.size());

	mCallingExpiredTimerTasks = true;
	mCancelingTimerTasks.clear();
	for (std::vector<TimerTaskPtr>::iterator it = expiredTimerTasks.begin(); it != expiredTimerTasks.end(); ++it) {
		assert(it->get());		
		it->get()->run();
	}

	mCallingExpiredTimerTasks = false;
	
	reset(expiredTimerTasks, now);
}

size_t Timer::getExpired(const Timestamp &now, std::vector<TimerTaskPtr> &expiredTimerTasks)
{
	size_t count = 0;
	TimerTaskList::iterator last = mTimerTasks.upper_bound(now.usecs());
	for (TimerTaskList::iterator it = mTimerTasks.begin() ; it != last; ++it) {
		expiredTimerTasks.push_back(it->second);		
		++count;
    }

	mTimerTasks.erase(mTimerTasks.begin(), last);

	return count;
}

bool Timer::add(const TimerTaskPtr &timerTask)
{		
	bool earliestChanged = false;
	const long whenUsec = timerTask->expiration().usecs();
	
	TimerTaskList::iterator it = mTimerTasks.begin();
    if (it == mTimerTasks.end() || whenUsec < it->first) {
        earliestChanged = true;
    }

	assert(find(whenUsec, timerTask.get()) == mTimerTasks.end());
	mTimerTasks.insert(std::pair<long, TimerTaskPtr>(whenUsec, timerTask));	

	return earliestChanged;
}

Timer::TimerTaskList::iterator Timer::find(long usec, const TimerTask *timerTask)
{
	TimerTaskList::iterator pos = mTimerTasks.find(usec);
	if (pos == mTimerTasks.end()) {
		return pos;
	}

	const size_t count = mTimerTasks.count(usec);
	assert(count > 0);

	for (size_t i = 0; i < count; ++i) {
		if (pos->second.get() == timerTask) {
			return pos;
		}
		++pos;
	}

	return mTimerTasks.end();
	
}

void Timer::reset(const std::vector<TimerTaskPtr> &expiredTimerTasks, const Timestamp &now)
{
	for (std::vector<TimerTaskPtr>::const_iterator it = expiredTimerTasks.begin(); it != expiredTimerTasks.end(); ++it) {
		if ( (it->get()->repeat()) && (mCancelingTimerTasks.find(it->get()) == mCancelingTimerTasks.end()) ) {

			it->get()->reset(now);
		    add(*it);
		}
	}

	if (!mTimerTasks.empty()) {
		long nextExpireUsec = mTimerTasks.begin()->first;
		if (nextExpireUsec > 0) {
			resetTimerfd(mTimerfd, Timestamp(nextExpireUsec));
		}
    }
}


