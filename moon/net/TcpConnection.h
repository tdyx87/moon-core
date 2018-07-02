/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_TCPCONNNECTION_H_
#define MOON_NET_TCPCONNNECTION_H_

#include <moon/net/InetAddress.h>
#include <moon/net/Socket.h>
#include <moon/net/Buffer.h>
#include <moon/net/Callbacks.h>
#include <moon/os/EventChannel.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <string>


namespace moon
{
class EventLoop;
namespace net
{

class TcpConnection : boost::noncopyable, public boost::enable_shared_from_this<TcpConnection>
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

	void send(const void* message, size_t len);
	void send(Buffer* message);
    
	void shutdown(); 

	Buffer& inputBuffer() {return mInputBuffer;}
	Buffer& outPutBuffer() {return mOutputBuffer;}
    
private:	
	void setConnectionCallback(const ConnectionCallback& cb) { mConnectionCallback = cb; }
    void setMessageCallback(const MessageCallback& cb) { mMessageCallback = cb; }
    void setCloseCallback(const CloseCallback& cb)  { mCloseCallback = cb; }

    void connectEstablished();	
    void connectDestroyed(); 

    virtual void onClose();
    virtual void onRead();
    virtual void onWrite();
    virtual void onError();
	  

	void sendInLoop();
	void sendInLoop(const void* message, size_t len);
    void shutdownInLoop();
private:
	friend class EventChannel;
    friend class TcpServer;

    State mState;  // connection state
    EventLoop *mEventLoop;
	std::string mName;  // connection name

	boost::scoped_ptr<Socket> mSocket;	
	boost::scoped_ptr<EventChannel> mEventChannel;

    const InetAddress mLocalAddress;
    const InetAddress mPeerAddress;
	
	Buffer mInputBuffer;
    Buffer mOutputBuffer; 
	Buffer mSaveDataBuffer;

	ConnectionCallback mConnectionCallback;  // 创建连接回调函数
	MessageCallback mMessageCallback;        // 消息可读回调函数
	CloseCallback mCloseCallback;            // 关闭连接回调函数
	
};




}  // ~net
}  // ~moon

#endif  // ~MOON_NET_TCP_CONNNECTION_H_