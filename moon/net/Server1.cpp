/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/net/Server1.h>

#include <moon/Logger.h>
#include <moon/ByteBuffer.h>
#include <moon/net/Connection.h>
#include <moon/net/IOThread.h>
#include <moon/net/SocketOps.h>
#include <moon/net/TcpAcceptor.h>

#include <moon/os/EventLoop.h>

#include <assert.h>

namespace moon
{
namespace net
{

using std::string;


void defaultConnectionCallback(const ConnectionPtr& conn)
{
	LOGGER_TRACE("fd[%d]name:%s %s -> %s is %s", conn->getFd(), conn->getName().c_str(), conn->getLocalAddress().toIpPort().c_str(), conn->getPeerAddress().toIpPort().c_str(),
		conn->isConnected()?"UP":"DOWN");
}

void defaultMessageCallback(const ConnectionPtr& conn, Buffer &buf)
{                      
	string data = buf.retrieveAllAsString();
    LOGGER_TRACE("defaultMessageCallback, name:%s buf:%s", conn->getName().c_str(), data.c_str());
	conn->shutdown();
}

Server1::Server1(EventLoop* loop, const InetAddress& listenAddr, const std::string& name)
	: mNameArg(name),
	  mEventLoop(loop),
	  mIOThread(new IOThread()),
	  mTcpAcceptor(new TcpAcceptor(loop, listenAddr, true))	 
{
	mNextConnId = 1;
	mIsStarted = false;

	mConnectionCallback = defaultConnectionCallback;
	mMessageCallback = defaultMessageCallback;

	mTcpAcceptor->setNewConnectionCallback(std::bind(&Server1::onNewConnection, this, _1, _2));
}

Server1::~Server1()
{
}

void Server1::setThreadNum(int numThreads)
{
	assert(0 <= numThreads);
    return ;
}

void Server1::start()
{
	if (!mIsStarted) {
		assert(!mTcpAcceptor->listenning());
		if (mTcpAcceptor->listen() != 0) {
			return ;
		}

		mIOThread->start();
	}

	mIsStarted = true;	
}

/** called when a connection is completed*/
void Server1::onNewConnection(int fd, const InetAddress& peerAddr)
{
    char szBuf[32] = {0};
    snprintf(szBuf, sizeof (szBuf), "-%d", mNextConnId);
    ++mNextConnId;
	const string connName = mNameArg + szBuf;

    InetAddress localAddr(sockets::getLocalAddr(fd));

	EventLoop* ioLoop = mIOThread->eventLoop();
	ConnectionPtr conn(new Connection(fd, connName, ioLoop, localAddr, peerAddr));

	conn->setConnectionCallback(mConnectionCallback);
	conn->setMessageCallback(mMessageCallback);
    
    mIOThread->post(conn);
}


}  // ~net
}  // ~moon