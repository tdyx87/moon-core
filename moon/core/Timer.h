/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_CORE_TIMER_H_
#define MOON_CORE_TIMER_H_

#include <moon/AtomicInteger.h>
#include <moon/core/EventChannel.h>
#include <moon/core/TimerTaskId.h>

#include <stdint.h>
#include <map>
#include <memory>
#include <set>
#include <vector>

namespace moon
{

class EventLoop;

/**
  一个Timer对象表示一个定时器，Timer对象需要运行在一个EventLoop中，  可以向一个Timer对象添加多个TimerTask。

  1 可以通过Timer::add()方法添加一个新的定时任务到该对象，该方法返回一个TimerTaskId对象。
  2 可以通过Timer::cancel()方法取消已加入该对象的定时任务，通过Timer::add()返回的TimerTaskId对象进行取消。

注意:
  定时任务的执行时间点并不一定就是其设置的准确时间点。

设计说明:
  1 通过timerfd_create创建一个文件描述符（成为定时器描述符），然后通过timerfd_settime设置该描述符读事件触发的时间，使用EventLoop+EventChannel模式监控该描述符的读事件。
  2 创建一个定时任务队列（TimerTaskList），该队列采用std::multimap作为底层实现，以'任务执行时间'为key，
    由于'任务执行时间'在时间上可能相同，因此采用std::multimap作为底层实现。
    当'定时器描述符'有读时间时，从该队列获取'到期的定时任务'（小于等于当前时间的定时任务），从该队列移除这些'到期的定时任务'，并执行这些'到期的定时任务'。
    '到期的定时任务'执行完毕后，再将'到期的定时任务'需要重复执行的任务重新加入到该任务队列。 

*/
class Timer
{
	typedef std::shared_ptr<TimerTask> TimerTaskPtr;
	typedef std::multimap<long, TimerTaskPtr> TimerTaskList; 
public:
	explicit Timer(EventLoop* loop);
	~Timer();

	/**
	  添加一个新的定时任务到该对象，该任务将在@when指定的时间（运行时间可能不会和when指定的完全一致）运行。
	  1 如果periodMsec为0，该定时任务只会执行一次。
	  2 如果periodMsec不为0，每隔periodMsec毫秒运行一次，如果count不为0，则执行count次以后才会停止。

	  @params cb 定时任务执行时会回调该函数对象。
	  @params when 一个Timestamp对象，表示定时任务第一次执行的时间点。
	  @params periodMsec 定时任务执行间隔，小于等于0表示该任务只执行一次，大于0表示该任务在第一次执行后，每隔periodMsec毫秒都会执行一次。
      @params count 定时任务循环执行的次数。
      
	  @return 返回一个TimerTaskId对象，表示该定时任务对应的ID，后续可以使用该值取消该定时任务。
	*/
	TimerTaskId add(const TimerCallback& cb, const Timestamp &when, long periodMsec = 0L, int count = 0);
	
	/**
	  取消@timerTaskId指定的定时任务。
	  如果@timerTaskId对应的定时任务存在且等待执行中，则会移除对应的定时任务，对应的任务以后不会再被执行。
	  如果@timerTaskId对应的定时任务存在且在执行中，等本次任务执行完毕将会取消该定时任务，该任务执行完成后不会再次执行。
	  如果@timerTaskId对应的定时任务不存在，调用本方法没有任务效果。
	*/
	void cancel(const TimerTaskId &timerTaskId);
private:
	void addTimerInLoop(TimerTask *timerTask);
	void cancelTimerInLoop(const TimerTaskId &timerTaskId);
	
	void handleRead();	

	/**
	  获取执行时间少于等于@now对象指定的定时任务，并将其保存在@expiredTimerTasks，并从定时队列将到期的定时任务移除。
	  @return 返回当前到期的定时任务数量
	*/
	size_t getExpired(const Timestamp &now, std::vector<TimerTaskPtr> &expiredTimerTasks);

	// 将定时任务@timerTask添加到定时任务队列中
	bool add(const TimerTaskPtr &timerTask);	

	/**
	  在定时任务队列mTimerTasks中查询执行时间为@usec，且定时任务为@timerTask的任务，
	  如果对应的定时任务存在，返回一个有效的TimerTaskList::iterator对象指向该定时任务，
	  如果对应的任务不存在，返回mTimerTasks.end()。
	*/
	TimerTaskList::iterator find(long usec, const TimerTask *timerTask);

	/**
	  将@expiredTimerTasks队列中需要重复执行的任务重新添加到定时队列中。
	*/
	void reset(const std::vector<TimerTaskPtr> &expiredTimerTasks, const Timestamp &now);	
private:
	static AtomicInt64 sNumCreated;
	
	EventLoop *mEventLoop;
	const int mTimerfd;	
	EventChannel mEventChannel;

	bool mCallingExpiredTimerTasks;
	TimerTaskList mTimerTasks;  

	// mCancelingTimerTasks用于保存需要取消的定时任务，如果定时任务在执行期间被取消（例如：在定时任务回调方法中调用取消接口取消该任务），
	// 此时需要将该定时任务保存到mCancelingTimerTasks中，在mCancelingTimerTasks中存在的定时任务不会再被添加到定时任务队列中。
	// mCancelingTimerTasks的主要目的是防止需要重复执行的定时任务被重新添加到定时任务队列中。
	std::set<TimerTask *> mCancelingTimerTasks; 
};

}  // ~moon

#endif
