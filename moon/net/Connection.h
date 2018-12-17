/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_CONNNECTION_H_
#define MOON_NET_CONNNECTION_H_

#include <moon/noncopyable.h>
#include <moon/net/Buffer.h>
#include <moon/net/NetDefine.h>
#include <moon/net/InetAddress.h>
#include <moon/net/Socket.h>
#include <moon/os/EventChannel.h>

#include <memory>
#include <string>

namespace moon
{
class EventLoop;

namespace net
{
class Connection : noncopyable, public std::enable_shared_from_this<Connection>
{
	// states of the connection
	enum State { kDisconnected, kConnecting, kConnected, kDisconnecting };
public:
	Connection(int sockfd, const std::string& name, EventLoop* loop,
	const InetAddress& localAddr, const InetAddress& peerAddr);
    ~Connection();
    
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
	void setConnectionCallback(const ConnectionCallback1& cb) { mConnectionCallback = cb; }
    void setMessageCallback(const MessageCallback1& cb) { mMessageCallback = cb; }
    void setCloseCallback(const CloseCallback1& cb)  { mCloseCallback = cb; }

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
    friend class IOThread;
    friend class Server1;

    State mState;  // connection state
    EventLoop *mEventLoop;
	std::string mName;  // connection name

	std::unique_ptr<Socket> mSocket;	
	std::unique_ptr<EventChannel> mEventChannel;

    const InetAddress mLocalAddress;
    const InetAddress mPeerAddress;
	
	Buffer mInputBuffer;
    Buffer mOutputBuffer; 
	Buffer mSaveDataBuffer;

	ConnectionCallback1 mConnectionCallback;  // 创建连接回调函数
	MessageCallback1 mMessageCallback;        // 消息可读回调函数
	CloseCallback1 mCloseCallback;            // 关闭连接回调函数
	
};




}  // ~net
}  // ~moon

#endif  // ~MOON_NET_TCP_CONNNECTION_H_