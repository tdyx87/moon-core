/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_TCPCONNNECTION_H_
#define MOON_NET_TCPCONNNECTION_H_

#include <moon/noncopyable.h>
#include <moon/net/Buffer.h>
#include <moon/net/NetDefine.h>
#include <moon/net/InetAddress.h>
#include <moon/net/Socket.h>
#include <moon/core/EventChannel.h>

#include <boost/any.hpp>
#include <memory>
#include <string>

namespace moon
{
class EventLoop;

namespace net
{
class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection>
{
	// states of the connection
	enum State { kDisconnected, kConnecting, kConnected, kDisconnecting };
public:
	TcpConnection(int sockfd, const std::string& name, EventLoop* loop,
	const InetAddress& localAddr, const InetAddress& peerAddr);
    ~TcpConnection();
    
    EventLoop* getLoop()const {return mEventLoop;}
	int getFd() const {return mEventChannel->getFd();}
    
	const std::string& getName()const {return mName;}
	const InetAddress& getLocalAddress() const { return mLocalAddress; }
    const InetAddress& getPeerAddress() const { return mPeerAddress; }    
	bool isConnected() const { return kConnected == mState; }
    
    void send(const void *message);
	void send(const void *message, size_t len);
	void send(Buffer* message);
    
	void shutdown(); 

	Buffer& inputBuffer() {return mInputBuffer;}
	Buffer& outPutBuffer() {return mOutputBuffer;}

	void setContext(const boost::any& context) {mContext = context;}
	const boost::any& getContext()const {return mContext;}
	boost::any* getMutableContext(){ return &mContext; }
    
private:	
	void setConnectionCallback(const OnConnectionCallback& cb) { mConnectionCb = cb; }
    void setMessageCallback(const OnMessageCallback& cb) { mMessageCb = cb; }
    void setCloseCallback(const OnCloseCallback& cb)  { mCloseCb = cb; }

    void connectEstablished();	
    void connectDestroyed(); 

    void handleClose();
    void handleRead();
    void handleWrite();
    void handleError();
	  
	void sendInLoop();
	void sendInLoop(const void* message, size_t len);
    void shutdownInLoop();
private:
	friend class EventChannel;
    friend class TcpServer;
    friend class Server;

    State mState;  
    EventLoop *mEventLoop;
	std::string mName;  
	std::unique_ptr<Socket> mSocket;	
	std::unique_ptr<EventChannel> mEventChannel;
    const InetAddress mLocalAddress;
    const InetAddress mPeerAddress;
	
	Buffer mInputBuffer;
    Buffer mOutputBuffer; 
	Buffer mSaveDataBuffer;

	OnConnectionCallback mConnectionCb;  // 创建连接回调函数
	OnMessageCallback mMessageCb;        // 消息可读回调函数
	OnCloseCallback mCloseCb;            // 关闭连接回调函数

	boost::any mContext;
};




}  // ~net
}  // ~moon

#endif  // ~MOON_NET_TCP_CONNNECTION_H_