/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/net/Connection.h>

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

Connection::Connection(int sockfd, const string& name, EventLoop* loop, const InetAddress& localAddr, const InetAddress& peerAddr) :
    mEventLoop(loop), 
    mName(name), 
    mSocket(new Socket(sockfd)), 
	mEventChannel(new EventChannel(sockfd, loop)),
	mLocalAddress(localAddr),
    mPeerAddress(peerAddr)
{
	mState = kConnecting;
	LOGGER_TRACE("TcpConnectio::create. state:%d", mState);
	
	mEventChannel->setReadCallback(std::bind(&Connection::onRead, this));
	mEventChannel->setWriteCallback(std::bind(&Connection::onWrite, this));
	mEventChannel->setCloseCallback(std::bind(&Connection::onClose, this));
	mEventChannel->setErrorCallback(std::bind(&Connection::onError, this));    
}

Connection::~Connection()
{		
    LOGGER_TRACE("Connection::destroy. state:%d", mState);
	assert(kDisconnected == mState);
}

void Connection::connectEstablished()
{
	assert(kConnecting == mState);
	mState = kConnected;

	mEventChannel->tie(shared_from_this());
	mEventChannel->enableReading();	
	mConnectionCallback(shared_from_this());	
}

void Connection::connectDestroyed()
{
	if (kConnected == mState) {
		mState = kDisconnected;
		mEventChannel->remove();
		mConnectionCallback(shared_from_this());	
	}
	
	// do nothing
}

void Connection::onRead()
{
	const ssize_t iRead = mInputBuffer.readFd(mEventChannel->getFd());
	if (iRead > 0) {
		if (NULL != mMessageCallback) {
			mMessageCallback(shared_from_this(), mInputBuffer);
		}
	} else if (0 == iRead) {
		this->onClose();
	} else {
		LOGGER_ERROR("Connection::onRead failed.");
		this->onClose();
	}	
}

void Connection::onWrite()
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
		LOGGER_ERROR("Connection::onWrite");		
		this->onClose();
	}
}

void Connection::onClose()
{
	assert( (kConnected == mState) || (kDisconnecting == mState) );

	mState = kDisconnected;
	mEventChannel->remove();
    
	ConnectionPtr guardThis(shared_from_this());
	mConnectionCallback(guardThis);	 // 回调连接回调函数,通知用户连接即将关闭	
	mCloseCallback(guardThis);       // 执行关闭回调函数
}

void Connection::onError()
{
    int err = sockets::getSocketError(mEventChannel->getFd());
	LOGGER_ERROR("Connection::onError name=%s %s", mName.c_str(), strerror(err)); 
}

/** 发送数据 */
void Connection::send(const void* message, size_t len)
{
	if (kConnected == mState) {
		if (mEventLoop->isInLoopThread()) {
			sendInLoop(message, len);
		} else {
			mSaveDataBuffer.append(static_cast<const char*>(message), len);
			void (Connection::*fp)() = &Connection::sendInLoop;
			mEventLoop->runInLoop(std::bind(fp, this));
		}
	}	
}

void Connection::send(Buffer* buf)
{
	if (kConnected == mState) {
		if (mEventLoop->isInLoopThread()) {
			sendInLoop(buf->data(), buf->readableBytes());
			buf->retrieveAll();
		} else {
			buf->swap(mSaveDataBuffer);
			buf->retrieveAll();
			void (Connection::*fp)() = &Connection::sendInLoop;
			mEventLoop->runInLoop(std::bind(fp, this));
		}
	}
}

void Connection::sendInLoop()
{
	sendInLoop(mSaveDataBuffer.data(), mSaveDataBuffer.readableBytes());
	mSaveDataBuffer.retrieveAll();
}

void Connection::sendInLoop(const void* message, size_t len)
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
				LOGGER_ERROR("Connection::send:%s", strerror(errno));
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

void Connection::shutdown()
{
	if (kConnected == mState) {
		mState = kDisconnecting;	
		mEventLoop->runInLoop(std::bind(&Connection::shutdownInLoop, this));		
	}	
}

void Connection::shutdownInLoop()
{
	mEventLoop->assertInLoopThread();
    if (!mEventChannel->isWriting()) {
        // we are not writing
        mSocket->shutdownWrite();
    }
}


}  // ~net
}  // ~moon