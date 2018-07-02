#include <moon/os/Poller.h>
#include <moon/os/EventChannel.h>

#include <stdio.h>
#include <poll.h>
#include <stdexcept>
#include <stdio.h>

using namespace moon;

Poller::Poller(EventLoop* loop) : mEventLoop(loop)
{
}

Poller::~Poller()
{
}


bool Poller::hasEventChannel(EventChannel* channel) const
{
    this->assertInLoopThread();

	EventChannelMap::const_iterator it = mEventChannels.find(channel->getFd());
    return it != mEventChannels.end() && it->second == channel;
}
