/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/net/Server.h>

#include <moon/Logger.h>
#include <moon/net/Buffer.h>
#include <moon/net/SocketOps.h>
#include <moon/net/TcpAcceptor.h>
#include <moon/net/TcpConnection.h>
#include <moon/os/EventLoop.h>
#include <moon/os/EventLoopThreadPool.h>
#include <moon/os/ServiceConfig.h>

#include <assert.h>

namespace moon
{
namespace net
{

using std::string;


void defaultConnectionCallback(const TcpConnectionPtr& conn)
{
	LOGGER_TRACE("fd[%d]name:%s %s -> %s is %s", conn->getFd(), conn->getName().c_str(), conn->getLocalAddress().toIpPort().c_str(), conn->getPeerAddress().toIpPort().c_str(),
		conn->isConnected()?"UP":"DOWN");
}

void defaultMessageCallback(const TcpConnectionPtr& conn, Buffer &buf)
{                      
	string data = buf.retrieveAllAsString();
    LOGGER_TRACE("defaultMessageCallback, name:%s buf:%s", conn->getName().c_str(), data.c_str());
	conn->shutdown();
}

Server::Server(EventLoop* loop) : mEventLoop(loop)
{
	mNextConnId = 1;
	mIsStarted = false;

	mConnectionCallback = defaultConnectionCallback;
	mMessageCallback = defaultMessageCallback;
}

Server::~Server()
{
}

void Server::start(const ServiceConfig &config)
{
	InetAddress listenAddr(config.serviceHostname().c_str(), config.servicePort());
	mThreadPool = std::shared_ptr<EventLoopThreadPool>(new EventLoopThreadPool(mEventLoop));
	mTcpAcceptor = std::unique_ptr<TcpAcceptor>(new TcpAcceptor(mEventLoop, listenAddr, true));	 
	mTcpAcceptor->setNewConnectionCallback(std::bind(&Server::onNewConnection, this, _1, _2));
    mThreadPool->setThreadNum(config.ioThreadNum());

    if (!mIsStarted) {
		assert(!mTcpAcceptor->listenning());
		if (mTcpAcceptor->listen() != 0) {
			return ;
		}

		mThreadPool->start(NULL);
	}

	mIsStarted = true;	
}

/** called when a connection is completed*/
void Server::onNewConnection(int fd, const InetAddress& peerAddr)
{
    char szBuf[32] = {0};
    snprintf(szBuf, sizeof (szBuf), "-%d", mNextConnId);
    ++mNextConnId;
	const string connName = mNameArg + szBuf;

    InetAddress localAddr(sockets::getLocalAddr(fd));

	EventLoop* ioLoop = mThreadPool->getNextLoop();
	TcpConnectionPtr conn(new TcpConnection(fd, connName, ioLoop, localAddr, peerAddr));
	mTcpConnection[connName] = conn;

	conn->setConnectionCallback(mConnectionCallback);
	conn->setMessageCallback(mMessageCallback);
	conn->setCloseCallback(std::bind(&Server::onRemoveConnection, this, _1));
    
	// 一定要使用runInLoop,不要直接调用connectEstablished,因为可能不在当前loop所在的线程
	ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));  
}

void Server::onRemoveConnection(const TcpConnectionPtr& conn)
{
	mEventLoop->runInLoop(std::bind(&Server::onRemoveConnectionInLoop, this, conn));
}

void Server::onRemoveConnectionInLoop(const TcpConnectionPtr& conn)
{
	LOGGER_INFO("Server::removeConnection [%s] - connection %s size:%ld", mNameArg.c_str(), conn->getName().c_str(), mTcpConnection.size());
    
	size_t n = mTcpConnection.erase(conn->getName());
	assert(1 == n);

	EventLoop* ioLoop = conn->getLoop();
    
	// 一定要使用queueInLoop,不要直接调用connectDestroyed()
	ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));  

	//conn->connectDestroyed();
}

}  // ~net
}  // ~moon