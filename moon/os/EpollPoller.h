/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_OS_EPOLLPOLLER_H_
#define MOON_OS_EPOLLPOLLER_H_

/**
* 文件名: EpollPoller.h
* 文件描述: 
*/

#include <moon/os/Poller.h>

#include <sys/epoll.h>

namespace moon
{

/**
  继承自@Poller类，采用epoll作为事件监控的底层实现。
*/
class EpollPoller : public Poller
{
public:	
	EpollPoller(EventLoop* loop);
    virtual ~EpollPoller();

	virtual int poll(int timeoutMs, EventChannelList* activeEventChannels);

	virtual void updateEventChannel(EventChannel* eventChannel);

	virtual void removeEventChannel(EventChannel* eventChannel); 
	
private:
    void fillActiveEvents(int events, EventChannelList* activeEventChannels);

	void update(int operation, EventChannel* eventChannel);
private:
	int mEpollFd;

	typedef std::vector<struct epoll_event> EventList;
    EventList mEvents;    
	
};


}  // ~moon
#endif  // ~