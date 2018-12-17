/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/Logger.h>
#include <moon/net/IOThread.h>
#include <moon/net/Connection.h>
#include <moon/os/EventLoop.h>
#include <moon/os/EventLoopThread.h>

#include <assert.h>

namespace moon
{
namespace net
{

IOThread::IOThread()
{
    mLoopThread = std::unique_ptr<EventLoopThread>(new EventLoopThread("IO-Thread", NULL));
}

IOThread::~IOThread()
{
    mConnections.clear();
}

EventLoop* IOThread::eventLoop()
{
	return mEventLoop;
}

void IOThread::post(const ConnectionPtr &connection)
{
    mEventLoop->runInLoop(std::bind(&IOThread::onNewConnection, this, connection));
}

bool IOThread::quit()
{
	mLoopThread->quit();
	return true;
}

void IOThread::onNewConnection(const ConnectionPtr &connection)
{
    assert(mConnections.find(connection->getName()) == mConnections.end());

    connection->setCloseCallback(std::bind(&IOThread::onRemoveConnection, this, _1));
    mConnections[connection->getName()] = connection;
    connection->connectEstablished();
}

void IOThread::onRemoveConnection(const ConnectionPtr& conn)
{
	LOGGER_INFO("IOThread::onRemoveConnection - connection %s size:%zu", conn->getName().c_str(), mConnections.size());
    
	size_t n = mConnections.erase(conn->getName());
	assert(1 == n);
    mEventLoop->deleteLater(conn);
}

void IOThread::start()
{
    mEventLoop = mLoopThread->startLoop();
}

}  // ~net	
}  // ~moon