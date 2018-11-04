#include <moon/net/TcpServer.h>
#include <moon/net/SocketOps.h>
#include <moon/net/Buffer.h>
#include <moon/net/TcpAcceptor.h>
#include <moon/net/TcpConnection.h>
#include <moon/logger/Logger.h>
#include <moon/os/EventLoop.h>
#include <moon/os/EventLoopThreadPool.h>

#include <boost/bind.hpp>

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

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string& name)
	: mNameArg(name),
	  mEventLoop(loop),
	  mThreadPool(new EventLoopThreadPool(loop)),
	  mTcpAcceptor(new TcpAcceptor(loop, listenAddr, true))	 
{
	mNextConnId = 1;
	mIsStarted = false;

	mConnectionCallback = defaultConnectionCallback;
	mMessageCallback = defaultMessageCallback;

	mTcpAcceptor->setNewConnectionCallback(boost::bind(&TcpServer::onNewConnection, this, _1, _2));
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
	if (!mIsStarted)
	{
		assert(!mTcpAcceptor->listenning());
		if (mTcpAcceptor->listen() != 0)
		{
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

	conn->setConnectionCallback(mConnectionCallback);
	conn->setMessageCallback(mMessageCallback);
	conn->setCloseCallback(boost::bind(&TcpServer::onRemoveConnection, this, _1));
    
	// 一定要使用runInLoop,不要直接调用connectEstablished,因为可能不在当前loop所在的线程
	ioLoop->runInLoop(boost::bind(&TcpConnection::connectEstablished, conn));  
}

void TcpServer::onRemoveConnection(const TcpConnectionPtr& conn)
{
	mEventLoop->runInLoop(boost::bind(&TcpServer::onRemoveConnectionInLoop, this, conn));
}

void TcpServer::onRemoveConnectionInLoop(const TcpConnectionPtr& conn)
{
	LOGGER_INFO("TcpServer::removeConnection [%s] - connection %s size:%ld", mNameArg.c_str(), conn->getName().c_str(), mTcpConnection.size());
    
	size_t n = mTcpConnection.erase(conn->getName());
	assert(1 == n);

	EventLoop* ioLoop = conn->getLoop();
    
	// 一定要使用queueInLoop,不要直接调用connectDestroyed()
	ioLoop->queueInLoop(boost::bind(&TcpConnection::connectDestroyed, conn));  

	//conn->connectDestroyed();
}

}  // ~net
}  // ~moon