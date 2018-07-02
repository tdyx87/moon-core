#include <moon/net/TcpConnection.h>
#include <moon/net/SocketOps.h>
#include <moon/logger/Logger.h>
#include <moon/os/EventLoop.h>

#include <boost/bind.hpp>

#include <assert.h>
#include <errno.h>
#include <string.h>

using std::string;

namespace moon
{
namespace net
{

TcpConnection::TcpConnection(int sockfd, const string& name, EventLoop* loop, const InetAddress& localAddr, const InetAddress& peerAddr) :
    mEventLoop(loop), 
    mName(name), 
    mSocket(new Socket(sockfd)), 
	mEventChannel(new EventChannel(sockfd, loop)),
	mLocalAddress(localAddr),
    mPeerAddress(peerAddr)
{
	mState = kConnecting;
	LOGGER_TRACE("TcpConnectio::create. state:%d", mState);
	
	mEventChannel->setReadCallback(boost::bind(&TcpConnection::onRead, this));
	mEventChannel->setWriteCallback(boost::bind(&TcpConnection::onWrite, this));
	mEventChannel->setCloseCallback(boost::bind(&TcpConnection::onClose, this));
	mEventChannel->setErrorCallback(boost::bind(&TcpConnection::onError, this));    
}

TcpConnection::~TcpConnection()
{		
    LOGGER_TRACE("TcpConnectio::destroy. state:%d", mState);
	assert(kDisconnected == mState);
}

void TcpConnection::connectEstablished()
{
	assert(kConnecting == mState);
	mState = kConnected;

	mEventChannel->tie(shared_from_this());
	mEventChannel->enableReading();	
	mConnectionCallback(shared_from_this());	
}

void TcpConnection::connectDestroyed()
{
//    LOGGER_TRACE("TcpConnection::connectDestroyed. state:%d", mState);
	if (kConnected == mState)
	{
		mState = kDisconnected;
		mEventChannel->remove();
		mConnectionCallback(shared_from_this());	
	}
	
	// do nothing
}

void TcpConnection::onRead()
{
//	LOGGER_TRACE("TcpConnectio::onRead. state:%d", mState);
	const ssize_t iRead = mInputBuffer.readFd(mEventChannel->getFd());
	if (iRead > 0)
	{
		if (NULL != mMessageCallback)
		{
			mMessageCallback(shared_from_this(), mInputBuffer);
		}
	}
	else if (0 == iRead)
	{
		this->onClose();
	}
	else
	{
		LOGGER_ERROR("TcpConnection::onRead failed.");
		this->onClose();
	}	
}

void TcpConnection::onWrite()
{	
//	LOGGER_TRACE("TcpConnectio::onWrite. state:%d", mState);
	const ssize_t iWrite = sockets::write(mEventChannel->getFd(), mOutputBuffer.peekFirst(), mOutputBuffer.readableBytes());
    if (iWrite >= 0)
	{
		mOutputBuffer.retrieve(iWrite);
		if (0 == mOutputBuffer.readableBytes())
		{
			mEventChannel->disableWriting();       

			// if the state is kDisconnecting, which means this connection is expected to be close when the writing completed
			if (kDisconnecting == mState)
			{
				this->shutdownInLoop();
			}
		}
	}
	else
	{
		LOGGER_ERROR("TcpConnection::onWrite");		
		this->onClose();
	}
}

void TcpConnection::onClose()
{
//    LOGGER_TRACE("TcpConnection::onClose. state:%d", mState);
	assert( (kConnected == mState) || (kDisconnecting == mState) );

	mState = kDisconnected;
	mEventChannel->remove();
    
	TcpConnectionPtr guardThis(shared_from_this());
	mConnectionCallback(guardThis);	 // 回调连接回调函数,通知用户连接即将关闭	
	mCloseCallback(guardThis);       // 执行关闭回调函数
}

void TcpConnection::onError()
{
    int err = sockets::getSocketError(mEventChannel->getFd());
	LOGGER_ERROR("TcpConnection::onError name=%s %s", mName.c_str(), strerror(err)); 
}

/** 发送数据 */
void TcpConnection::send(const void* message, size_t len)
{
//	LOGGER_TRACE("TcpConnectio::send state:%d", mState);
	if (kConnected == mState)
	{
		if (mEventLoop->isInLoopThread())
		{
			sendInLoop(message, len);
		}
		else
		{
			mSaveDataBuffer.append(static_cast<const char*>(message), len);
			mEventLoop->runInLoop(boost::bind(&TcpConnection::sendInLoop, this));
		}
	}	
}

void TcpConnection::send(Buffer* buf)
{
//	LOGGER_TRACE("TcpConnectio::send1 state:%d", mState);
	if (kConnected == mState)
	{
		if (mEventLoop->isInLoopThread())
		{
			sendInLoop(buf->peekFirst(), buf->readableBytes());
			buf->retrieveAll();
		}
		else
		{
			buf->swap(mSaveDataBuffer);
			buf->retrieveAll();
			mEventLoop->runInLoop(boost::bind(&TcpConnection::sendInLoop, this));
		}
	}
}

void TcpConnection::sendInLoop()
{
//	LOGGER_TRACE("TcpConnectio::sendInLoop state:%d", mState);
	sendInLoop(mSaveDataBuffer.peekFirst(), mSaveDataBuffer.readableBytes());
	mSaveDataBuffer.retrieveAll();
}

void TcpConnection::sendInLoop(const void* message, size_t len)
{
//	LOGGER_TRACE("TcpConnectio::sendInLoop1 len[%d]state:%d", len, mState);
	mEventLoop->assertInLoopThread();

	ssize_t iWrite = 0;
	size_t remaining = len;

	// if can send directly
	bool sendDirectly = (!mEventChannel->isWriting()) && (mOutputBuffer.readableBytes() == 0);
	if (sendDirectly)
	{
		iWrite = sockets::write(mEventChannel->getFd(), message, len);
		//LOGGER_TRACE("sendInLoop send directly, iWrite[%d]", iWrite);
		if (iWrite >= 0)
		{
			remaining = len - iWrite;		
		}	
		else
		{
			iWrite = 0;
			if (errno != EWOULDBLOCK)
			{
				LOGGER_ERROR("TcpConnection::send:%s", strerror(errno));
				return ;
			}			
		}
	} // ~if (sendDirectly)
    
	assert(remaining <= len);
	

	if (remaining > 0)
	{
		LOGGER_TRACE("sendInLoop send not directly");
		// append the remaining to m_objOutputBuffer 
		mOutputBuffer.append(static_cast<const char*>(message) + iWrite, remaining);
		if (!mEventChannel->isWriting())
		{
			mEventChannel->enableWriting();
		}
	}	
}

void TcpConnection::shutdown()
{
	if (kConnected == mState)
	{
		mState = kDisconnecting;	
		mEventLoop->runInLoop(boost::bind(&TcpConnection::shutdownInLoop, this));		
	}	
}

void TcpConnection::shutdownInLoop()
{
//    LOGGER_TRACE("TcpConnection::shutdownInLoop. state:%d", mState);
	mEventLoop->assertInLoopThread();
    if (!mEventChannel->isWriting())
    {
        // we are not writing
        mSocket->shutdownWrite();
    }
}


}  // ~net
}  // ~moon