/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_SERVER_H_
#define MOON_NET_SERVER_H_

#include <moon/noncopyable.h>
#include <moon/net/NetDefine.h>

#include <string>
#include <map>

namespace moon
{
class EventLoop;
class EventLoopThreadPool;

namespace net
{
class InetAddress;
class TcpAcceptor;
class TcpConnection;

class Server : noncopyable
{
	typedef std::function<void(EventLoop*)> ThreadInitCallback;
    typedef std::map<std::string, TcpConnectionPtr> TcpConnectionMap;
public:
	Server(EventLoop* loop, const InetAddress& listenAddr, const std::string& name);
    ~Server();	
    
	const std::string& getName() const { return mNameArg; }
	EventLoop* getLoop() const { return mEventLoop; }
  
	void setThreadNum(int numThreads);
    void setThreadInitCallback(const ThreadInitCallback& cb){ mThreadInitCallback = cb; }

	void start();

	void setConnectionCallback(const OnConnectionCallback& cb){ mConnectionCb = cb; }
	void setMessageCallback(const OnMessageCallback& cb){ mMessageCb = cb; }

	// for test
	size_t connecionSize()const {return mTcpConnection.size();}
private:
	/** Called when a connection is completed, this call is not thread safe, but in loop */
	void onNewConnection(int fd, const InetAddress& peerAddr);

	/** Called when a connection is disconnected, this call is thread safe */
    void onRemoveConnection(const TcpConnectionPtr& conn);
    
	/** Called when a connection is disconnected, this call is not thread safe, but in loop */
	void onRemoveConnectionInLoop(const TcpConnectionPtr& conn);
private:
	std::string mNameArg;
    int mNextConnId;
	EventLoop* mEventLoop;   
	bool mIsStarted;

	OnConnectionCallback mConnectionCb;
    OnMessageCallback mMessageCb;
	ThreadInitCallback mThreadInitCallback;
    
	std::shared_ptr<EventLoopThreadPool> mThreadPool;
	std::unique_ptr<TcpAcceptor> mTcpAcceptor; 
	TcpConnectionMap mTcpConnection;
};





}  // ~net
}  // ~moon
#endif  // ~MOON_NET_TCPSERVER_H_