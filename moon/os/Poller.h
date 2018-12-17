/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou

  Poller用于监控EventChannel对象上的事件变化，一个Poller对象可以监控多个EventChannel对象，每个Poller对象
  都维护一个EventChannel队列，用于保存正在监控的EventChannel对象。
  每个Poller对象都在而且只能在一个EventLoop对象之中，每个Poller对象的所有方法都应该在对应的EventLoop中对象中执行。
  该类属于抽象类，无法直接使用，需要子类化该类，子类化该类必须实现如下3个方法：
  updateEventChannel()，removeEventChannel()，poll()()。

  updateEventChannel()方法用于添加新的EventChannel对象到Poller监控列表、更新现有的EventChannel对象的监控事件。

  removeEventChannel()方法用于将EventChannel对象从Poller对象中移出。

  poll()用于等待监控队列中有事件发生的EventChannel对象集，调用方线程将处于阻塞状态直到有一个或者多个EventChannel对象的事件被触发，或者超时时间已到。
  poll()方法返回时会设置当前有事件被触发的EventChannel对象集。
*/
#ifndef MOON_OS_POLLER_H_
#define MOON_OS_POLLER_H_

#include <moon/os/EventLoop.h>

#include <map>
#include <vector>

namespace moon
{
class EventChannel;

class Poller
{
protected:
	typedef std::map<int, EventChannel *> EventChannelMap;	
    EventChannelMap mEventChannels;

public:
	typedef std::vector<EventChannel *> EventChannelList;	

	Poller(EventLoop* loop);
    virtual ~Poller();

    static Poller* newDefaultPoller(EventLoop* loop);
    
	/**
	  此方法等待EventChannel列表中事件的触发，此方法将会阻塞调用方（如果设置了超时时间）直到有一个或者多个EventChannel对象有
	  事件被触发，或者超时时到。如果超时时间设置为0，则该方法不会阻塞，会直接返回，不管有没有事件被触发。如果超时时间设置为-1，
	  则表示一直等待直到有时间被触发。
	  
	  @params timeoutMs 等待超时时间，单位：毫秒。为0表示不等待，为-1表示永久等待。
	  @params activeChannels 用于保存本次有事件被触发的EventChannel对象集，不能为NULL。

	  @return 返回一个正整数表示有事件触发的EventChannel对象的数量；返回0表示无事件触发；返回-1表示有错误发生，请通过errno查看错误详情。
	*/
	virtual int poll(int timeoutMs, EventChannelList* activeChannels) = 0;
	    

	/**
	  更新EventChannel对象的监控事件。根据EventChannel对象的不同状态，Poller对象作出不同的处理。
	  当EventChannel处于EventChannel::kNew时，将EventChannel对象加入到监控队列，并监控EventChannel对象期望发送的事件；
	  当EventChannel处于EventChannel::kAdded时，更新EventChannel对象的事件，更新EventChannel对象期望发送的事件；
	  如果EventChannel对象上未设置任何期望监控的事件（即channel->isNoneEvent()返回true），此时将停止对EventChannel对象的监控，
	  但是并不会将该对象从监控队列中移除。

	  @params channel 一个指向EventChannel对象的指针，不能为NULL。
	*/
	virtual void updateEventChannel(EventChannel* channel) = 0;

	/**
	  将EventChannel对象从监控队列移除，一个被移除的EventChannel对象应该处于EventChannel::kNew或者EventChannel::kDeleted状态。
	  @params channel 一个指向EventChannel对象的指针，不能为NULL。
	*/
	virtual void removeEventChannel(EventChannel* channel) = 0;

	/**
	  返回true表示@channel存在于监控队列之中
	*/
	virtual bool hasEventChannel(EventChannel* channel) const;

	virtual size_t size() const {return mEventChannels.size();}

	/**
	  逻辑测试方法，内部使用。
	*/
	void assertInLoopThread() const
	{
	    mEventLoop->assertInLoopThread();
	}

private:
	EventLoop* mEventLoop;

};

}  // ~moon

#endif  // ~