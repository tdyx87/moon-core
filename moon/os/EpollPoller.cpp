/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/os/EpollPoller.h>
#include <moon/os/EventChannel.h>
#include <moon/Logger.h>
#include <moon/TypeCast.h>

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/poll.h>

// On Linux, the constants of poll(2) and epoll(4)
// are expected to be the same.
static_assert(EPOLLIN == POLLIN,        "epoll uses same flag values as poll");
static_assert(EPOLLPRI == POLLPRI,      "epoll uses same flag values as poll");
static_assert(EPOLLOUT == POLLOUT,      "epoll uses same flag values as poll");
static_assert(EPOLLRDHUP == POLLRDHUP,  "epoll uses same flag values as poll");
static_assert(EPOLLERR == POLLERR,      "epoll uses same flag values as poll");
static_assert(EPOLLHUP == POLLHUP,      "epoll uses same flag values as poll");

namespace moon
{

static const int kNew = -1;
static const int kAdded = 1;
static const int kDeleted = 2;
static const int kInitEventListSize = 32;

EpollPoller::EpollPoller(EventLoop* loop) : Poller(loop), mEvents(kInitEventListSize)
{
    mEpollFd = ::epoll_create1(EPOLL_CLOEXEC);
}

EpollPoller::~EpollPoller()
{
	::close(mEpollFd);
}

int EpollPoller::poll(int timeoutMs, EventChannelList* activeEventChannels)
{    
	const int events = ::epoll_wait(mEpollFd, &*mEvents.begin(), static_cast<int>(mEvents.size()), timeoutMs);
	if (events > 0) {
		fillActiveEvents(events, activeEventChannels);
		if (implicit_cast<size_t>(events) == mEvents.size()) {
            mEvents.resize(mEvents.size() * 2);
        }
    }
	
	return events;
}

void EpollPoller::fillActiveEvents(int events, EventChannelList* activeEventChannels)
{
	for (int i =0; i < events; ++i) {
		EventChannel* eventChannel = static_cast<EventChannel*>(mEvents[i].data.ptr);
		eventChannel->setActiveEvents(mEvents[i].events);
		activeEventChannels->push_back(eventChannel);				
	}
}

void EpollPoller::updateEventChannel(EventChannel* eventChannel)
{
	Poller::assertInLoopThread();
	const int state = eventChannel->getState();  // 获取通道状态
	if ( (state == kNew) || (state == kDeleted) ) {
	    // 一个新的事件通道,使用EPOLL_CTL_ADD添加到epoll中
	    int fd = eventChannel->getFd();	
		if (state == kNew) {
			assert(mEventChannels.find(fd) == mEventChannels.end());
	        mEventChannels[fd] = eventChannel;
		} else {
			LOGGER_TRACE("EpollPoller::updateEventChannel -> kDeleted.");
			assert(mEventChannels.find(fd) != mEventChannels.end());
			assert(mEventChannels[fd] == eventChannel);
	    }

	    // 添加到epoll队列
	    eventChannel->setState(kAdded);
	    update(EPOLL_CTL_ADD, eventChannel);  
	} else {
		const int fd = eventChannel->getFd();
		
		assert(state == kAdded);		    
		assert(mEventChannels.find(fd) != mEventChannels.end());
	    assert(mEventChannels[fd] == eventChannel);

		if (eventChannel->isNoneEvent()) {
			update(EPOLL_CTL_DEL, eventChannel);
			eventChannel->setState(kDeleted);
		}
        else {
		    update(EPOLL_CTL_MOD, eventChannel);
        }
	}		
}

void EpollPoller::removeEventChannel(EventChannel* eventChannel)
{
	Poller::assertInLoopThread();

	const int fd = eventChannel->getFd();  // 获取通道对应的描述符
	assert(mEventChannels.find(fd) != mEventChannels.end());
	assert(mEventChannels[fd] == eventChannel);
	assert(eventChannel->isNoneEvent());

	const int state = eventChannel->getState();  // 获取通道的状态
	assert( (state == kAdded) || (state == kDeleted) );	

	const size_t n = mEventChannels.erase(fd);  // 移除fd对于的通道
	assert(1 == n);
	if (state == kAdded) {
		update(EPOLL_CTL_DEL, eventChannel);
	}
	
	eventChannel->setState(kNew);
}

void EpollPoller::update(int operation, EventChannel* eventChannel)
{
	struct epoll_event event;

	bzero(&event, sizeof event);
	event.events = eventChannel->getCurrentEvents();
	event.data.ptr = eventChannel;

	int fd = eventChannel->getFd();
	if (::epoll_ctl(mEpollFd, operation, fd, &event) < 0) {
		LOGGER_FATAL("epoll_ctl op=%d fd=%d, %s", operation, fd, strerror(errno));
	}
}


}  // ~moon