#include <moon/os/EpollPoller.h>
#include <moon/os/EventChannel.h>
#include <moon/logger/Logger.h>
#include <moon/TypeCast.h>

#include <assert.h>
#include <errno.h>
#include <string.h>

using namespace moon;

//static const int kNew = -1;
//static const int kAdded = 1;
//static const int kDeleted = 2;
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
    //LOGGER_INFO("EpollPoller::poll events[%d]\n", events);
	if (events > 0)
    {
		fillActiveEvents(events, activeEventChannels);
		if (implicit_cast<size_t>(events) == mEvents.size())
        {
            mEvents.resize(mEvents.size() * 2);
        }
    }
	
	return events;
}

void EpollPoller::fillActiveEvents(int events, EventChannelList* activeEventChannels)
{
	for (int i =0; i < events; ++i)
	{
		EventChannel* eventChannel = static_cast<EventChannel*>(mEvents[i].data.ptr);
		//LOGGER_INFO("EpollPoller::fillActiveEvents eventChannel fd[%d]\n", eventChannel->getFd());
		eventChannel->setActiveEvents(mEvents[i].events);
		activeEventChannels->push_back(eventChannel);				
	}
}

void EpollPoller::updateEventChannel(EventChannel* eventChannel)
{
	Poller::assertInLoopThread();
	const EventChannel::State state = eventChannel->getState();  // 获取通道状态
	if ( (EventChannel::kNew == state) || (EventChannel::kDeleted == state) )
	{
	    // 一个新的事件通道,使用EPOLL_CTL_ADD添加到epoll中
	    int fd = eventChannel->getFd();	
		if (EventChannel::kNew == state)
		{
			assert(mEventChannels.find(fd) == mEventChannels.end());
	        mEventChannels[fd] = eventChannel;
		}
	    else
	    {
			LOGGER_TRACE("EpollPoller::updateEventChannel -> kDeleted.");
			assert(mEventChannels.find(fd) != mEventChannels.end());
			assert(mEventChannels[fd] == eventChannel);
	    }

	    // 添加到epoll队列
	    eventChannel->setState(EventChannel::kAdded);
	    update(EPOLL_CTL_ADD, eventChannel);  
	}
	else
	{
		const int fd = eventChannel->getFd();
		
		assert(EventChannel::kAdded == state);		    
		assert(mEventChannels.find(fd) != mEventChannels.end());
	    assert(mEventChannels[fd] == eventChannel);

		if (eventChannel->isNoneEvent())
		{
			update(EPOLL_CTL_DEL, eventChannel);
			eventChannel->setState(EventChannel::kDeleted);
		}
        else
        {
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

	const EventChannel::State state = eventChannel->getState();  // 获取通道的状态
	assert( (EventChannel::kAdded == state) || (EventChannel::kDeleted == state) );	

	const size_t n = mEventChannels.erase(fd);  // 移除fd对于的通道
	assert(1 == n);
	if (EventChannel::kAdded == state)
	{
		update(EPOLL_CTL_DEL, eventChannel);
	}
	
	eventChannel->setState(EventChannel::kNew);
}

void EpollPoller::update(int operation, EventChannel* eventChannel)
{
	struct epoll_event event;

	bzero(&event, sizeof event);
	event.events = eventChannel->getCurrentEvents();
	event.data.ptr = eventChannel;

	int fd = eventChannel->getFd();
	if (::epoll_ctl(mEpollFd, operation, fd, &event) < 0)
	{
		LOGGER_FATAL("epoll_ctl op=%d fd=%d, %s", operation, fd, strerror(errno));
	}
}

