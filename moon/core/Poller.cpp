/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/core/Poller.h>
#include <moon/core/EventChannel.h>
#include <moon/core/EpollPoller.h>

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

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    return new EpollPoller(loop);
}
