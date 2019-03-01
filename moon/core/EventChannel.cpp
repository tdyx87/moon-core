/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/Logger.h>
#include <moon/core/EventChannel.h>
#include <moon/core/EventLoop.h>

#include <assert.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>

namespace moon
{
const int EventChannel::kEventNone = 0;
const int EventChannel::kEventRead = POLLIN | POLLPRI;
const int EventChannel::kEventWrite = POLLOUT;

EventChannel::EventChannel(int fd, EventLoop* pEventLoop) : mFd(fd)
{    
	mCurEvents = kEventNone;
	mActiveEvents = kEventNone;
	mState = -1;

	mEventLoop = pEventLoop;
    mEventHandling = false;
	mTied = false;
}

/** 析构函数 */
EventChannel::~EventChannel()
{
	assert(!mEventHandling);
}

void EventChannel::remove()
{
	if (!isNoneEvent())
	{
		disableAll();
	}
	mEventLoop->removeEventChannel(this);
}

void EventChannel::handleEvent()
{
	std::shared_ptr<void> guard;
	if (mTied) {
	    guard = mTie.lock();
		if (!guard) {
			return ;
		}
	}

	mEventHandling = true;

	if ( (mActiveEvents & POLLHUP) && !(mActiveEvents & POLLIN) ) {
	    LOGGER_WARN("EventChannel::handleEvent() POLLHUP");
		if (mCloseCallback != NULL) {	
			mCloseCallback();
		}
	}

	if (mActiveEvents & (POLLERR | POLLNVAL)) {
	    if (mErrorCallback) {
			mErrorCallback();
		}
	}

	// 读事件处理
	if (mActiveEvents & (POLLIN | POLLPRI | POLLRDHUP) ) {
		if (mReadCallback != NULL) {
			mReadCallback();
		}
	}
    
	// 写事件处理
	if ( mActiveEvents & POLLOUT) {
		if (mWriteCallback != NULL) {
			mWriteCallback();
		}
	}	

	mEventHandling = false;
}

void EventChannel::update()
{
	mEventLoop->updateEventChannel(this);
}

void EventChannel::tie(const std::shared_ptr<void>& tie)
{
	mTie = tie;
	mTied = true;
}



}  // ~moon