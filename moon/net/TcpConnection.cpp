/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/net/TcpConnection.h>

#include <moon/Logger.h>
#include <moon/net/SocketOps.h>
#include <moon/os/EventLoop.h>

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
	
	mEventChannel->setReadCallback(std::bind(&TcpConnection::handleRead, this));
	mEventChannel->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
	mEventChannel->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
	mEventChannel->setErrorCallback(std::bind(&TcpConnection::handleError, this));    
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
	mConnectionCb(shared_from_this(), true);	
}

void TcpConnection::connectDestroyed()
{
	if (kConnected == mState) {
		mState = kDisconnected;
		mEventChannel->remove();
		mConnectionCb(shared_from_this(), false);	
	}
	
	// do nothing
}

void TcpConnection::handleRead()
{
	const ssize_t iRead = mInputBuffer.readFd(mEventChannel->getFd());
	if (iRead > 0) {
		mMessageCb(shared_from_this(), mInputBuffer);
	} else if (0 == iRead) {
		this->handleClose();
	} else {
		LOGGER_ERROR("TcpConnection::handleRead failed.");
		this->handleClose();
	}	
}

void TcpConnection::handleWrite()
{	
	const ssize_t iWrite = sockets::write(mEventChannel->getFd(), mOutputBuffer.data(), mOutputBuffer.readableBytes());
    if (iWrite >= 0) {
		mOutputBuffer.retrieve(iWrite);
		if (0 == mOutputBuffer.readableBytes()) {
			mEventChannel->disableWriting();       

			// if the state is kDisconnecting, which means this connection is expected to be close when the writing completed
			if (kDisconnecting == mState) {
				this->shutdownInLoop();
			}
		}
	} else {
		LOGGER_ERROR("TcpConnection::handleWrite");		
	}
}

void TcpConnection::handleClose()
{
	assert( (kConnected == mState) || (kDisconnecting == mState) );

	mState = kDisconnected;
	mEventChannel->remove();
    
	TcpConnectionPtr guardThis(shared_from_this());
	mConnectionCb(guardThis, false);	 // 回调连接回调函数,通知用户连接即将关闭	
	mCloseCb(guardThis);       // 执行关闭回调函数
}

void TcpConnection::handleError()
{
    int err = sockets::getSocketError(mEventChannel->getFd());
	LOGGER_ERROR("TcpConnection::onError name=%s %s", mName.c_str(), strerror(err)); 
}

void TcpConnection::send(const void *message)
{
    this->send(message, strlen(static_cast<const char*>(message)));
}

/** 发送数据 */
void TcpConnection::send(const void* message, size_t len)
{
	if (kConnected == mState) {
		if (mEventLoop->isInLoopThread()) {
			sendInLoop(message, len);
		} else {
			mSaveDataBuffer.append(static_cast<const char*>(message), len);
			void (TcpConnection::*fp)() = &TcpConnection::sendInLoop;
			mEventLoop->runInLoop(std::bind(fp, this));
		}
	}	
}

void TcpConnection::send(Buffer* buf)
{
	if (kConnected == mState) {
		if (mEventLoop->isInLoopThread()) {
			sendInLoop(buf->data(), buf->readableBytes());
			buf->retrieveAll();
		} else {
			buf->swap(mSaveDataBuffer);
			buf->retrieveAll();
			void (TcpConnection::*fp)() = &TcpConnection::sendInLoop;
			mEventLoop->runInLoop(std::bind(fp, this));
		}
	}
}

void TcpConnection::sendInLoop()
{
	sendInLoop(mSaveDataBuffer.data(), mSaveDataBuffer.readableBytes());
	mSaveDataBuffer.retrieveAll();
}

void TcpConnection::sendInLoop(const void* message, size_t len)
{
	mEventLoop->assertInLoopThread();

	ssize_t iWrite = 0;
	size_t remaining = len;

	// if can send directly
	bool sendDirectly = (!mEventChannel->isWriting()) && (mOutputBuffer.readableBytes() == 0);
	if (sendDirectly) {
		iWrite = sockets::write(mEventChannel->getFd(), message, len);
		if (iWrite >= 0) {
			remaining = len - iWrite;		
		} else {
			iWrite = 0;
			if (errno != EWOULDBLOCK) {
				LOGGER_ERROR("TcpConnection::send:%s", strerror(errno));
				return ;
			}			
		}
	} // ~if (sendDirectly)
    
	assert(remaining <= len);
	

	if (remaining > 0) {
		LOGGER_TRACE("sendInLoop send not directly");
		mOutputBuffer.append(static_cast<const char*>(message) + iWrite, remaining);
		if (!mEventChannel->isWriting()) {
			mEventChannel->enableWriting();
		}
	}	
}

void TcpConnection::shutdown()
{
	if (kConnected == mState) {
		mState = kDisconnecting;	
		mEventLoop->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));		
	}	
}

void TcpConnection::shutdownInLoop()
{
	mEventLoop->assertInLoopThread();
    if (!mEventChannel->isWriting()) {
        // we are not writing
        mSocket->shutdownWrite();
    }
}


}  // ~net
}  // ~moon