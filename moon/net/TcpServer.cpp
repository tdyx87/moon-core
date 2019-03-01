/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/net/TcpServer.h>

#include <moon/Logger.h>
#include <moon/net/Buffer.h>
#include <moon/net/SocketOps.h>
#include <moon/net/TcpAcceptor.h>
#include <moon/net/TcpConnection.h>
#include <moon/core/EventLoop.h>
#include <moon/core/EventLoopThreadPool.h>

#include <assert.h>

namespace moon
{
namespace net
{
using std::string;

void defaultConnectionCallback(const TcpConnectionPtr& conn, bool connected)
{
	LOGGER_TRACE("fd[%d]name:%s %s -> %s is %s", conn->getFd(), conn->getName().c_str(), conn->getLocalAddress().toIpPort().c_str(), conn->getPeerAddress().toIpPort().c_str(),
		connected ? "UP":"DOWN");
}

void defaultMessageCallback(const TcpConnectionPtr& conn, Buffer &buffer)
{                      
	string str = buffer.retrieveAllAsString();
    LOGGER_TRACE("defaultMessageCallback, name:%s buf:%s", conn->getName().c_str(), str.c_str());
	conn->shutdown();
}

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string& name)
	: mNameArg(name),
	  mEventLoop(loop),
	  mThreadPool(new EventLoopThreadPool(loop)),
	  mTcpAcceptor(new TcpAcceptor(loop, listenAddr, true))	 
{
	mNextConnId = 1;
	mIsStarted = false;

	mConnectionCb = defaultConnectionCallback;
	mMessageCb = defaultMessageCallback;

	mTcpAcceptor->setNewConnectionCallback(std::bind(&TcpServer::onNewConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
}

void TcpServer::setThreadNum(int numThreads)
{
	assert(0 <= numThreads);
    mThreadPool->setThreadNum(numThreads);
}

void TcpServer::start()
{
	if (!mIsStarted) {
		assert(!mTcpAcceptor->listenning());
		if (mTcpAcceptor->listen() != 0) {
			return ;
		}

		mThreadPool->start(mThreadInitCallback);
	}

	mIsStarted = true;	
}

/** called when a connection is completed*/
void TcpServer::onNewConnection(int fd, const InetAddress& peerAddr)
{
    char szBuf[32] = {0};
    snprintf(szBuf, sizeof (szBuf), "-%d", mNextConnId);
    ++mNextConnId;
	const string connName = mNameArg + szBuf;

    InetAddress localAddr(sockets::getLocalAddr(fd));

	EventLoop* ioLoop = mThreadPool->getNextLoop();
	TcpConnectionPtr conn(new TcpConnection(fd, connName, ioLoop, localAddr, peerAddr));
	mTcpConnection[connName] = conn;

	conn->setConnectionCallback(mConnectionCb);
	conn->setMessageCallback(mMessageCb);
	conn->setCloseCallback(std::bind(&TcpServer::onRemoveConnection, this, _1));
    
	// 一定要使用runInLoop,不要直接调用connectEstablished,因为可能不在当前loop所在的线程
	ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));  
}

void TcpServer::onRemoveConnection(const TcpConnectionPtr& conn)
{
	mEventLoop->runInLoop(std::bind(&TcpServer::onRemoveConnectionInLoop, this, conn));
}

void TcpServer::onRemoveConnectionInLoop(const TcpConnectionPtr& conn)
{
	LOGGER_INFO("TcpServer::removeConnection [%s] - connection %s size:%ld", mNameArg.c_str(), conn->getName().c_str(), mTcpConnection.size());
    
	size_t n = mTcpConnection.erase(conn->getName());
	assert(1 == n);

	EventLoop* ioLoop = conn->getLoop();
    
	// 一定要使用queueInLoop,不要直接调用connectDestroyed()
	ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));  

	//conn->connectDestroyed();
}

}  // ~net
}  // ~moon