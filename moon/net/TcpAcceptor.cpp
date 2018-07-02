#include <moon/net/TcpAcceptor.h>
#include <moon/net/SocketOps.h>
#include <moon/logger/Logger.h>
#include <moon/os/EventLoop.h>
#include <boost/bind.hpp>

#include <errno.h>
#include <string.h>

using namespace moon;
using namespace moon::net;

TcpAcceptor::TcpAcceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport) :
mServerSocket(listenAddr, true, reuseport), mAcceptChannel(mServerSocket.getFd(), loop)
{
	mEventLoop = loop;
	mIsListenning = false;
    mAcceptChannel.setReadCallback(boost::bind(&TcpAcceptor::handleRead, this));   
}

TcpAcceptor::~TcpAcceptor()
{
	mAcceptChannel.remove();
}

int TcpAcceptor::listen()
{
	if (mServerSocket.listen() != 0)
	{
		LOGGER_FATAL("listen error:%s fd:%d", strerror(errno), mServerSocket.getFd());
		return -1;
	}

	mIsListenning = true;
	mAcceptChannel.enableReading();  // 开启监听读事件

	return 0;
}

void TcpAcceptor::handleRead()
{
	InetAddress peerAddr;

	int connfd = mServerSocket.accept(&peerAddr);
	if (connfd >= 0)
	{
		if (NULL != mNewConnectionCallback)
		{
			mNewConnectionCallback(connfd, peerAddr);
		}
		else
		{
			LOGGER_TRACE("close connfd:%d", connfd);
			sockets::close(connfd);
		}
	}
	else
	{
		LOGGER_ERROR("accept failed, errno:%d error:%s", errno, strerror(errno));
		if (EMFILE == errno)
		{
			LOGGER_FATAL("the file descriptor has run out!");
		}
	}
}
