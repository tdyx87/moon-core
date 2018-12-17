/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_IOTHREAD_H_
#define MOON_NET_IOTHREAD_H_

#include <moon/net/NetDefine.h>

namespace moon
{
class EventLoop;
class EventLoopThread;

namespace net
{

class IOThread
{
public:
	IOThread();
	~IOThread();
    
    EventLoop* eventLoop();
	void post(const ConnectionPtr &connection);

	bool quit();

	void start();
private:
	void onNewConnection(const ConnectionPtr &connection);
	void onRemoveConnection(const ConnectionPtr& conn);
private:
	EventLoop *mEventLoop;
	std::unique_ptr<EventLoopThread> mLoopThread;
	ConnectionMap mConnections;
	uint64_t mAccepted;
	uint64_t mPending;

};  // ~IOThread

}  // ~net
}  // ~moon
#endif