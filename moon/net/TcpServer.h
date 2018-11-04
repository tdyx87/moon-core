/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_TCPSERVER_H_
#define MOON_NET_TCPSERVER_H_

#include <moon/net/Callbacks.h>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

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

class TcpServer : boost::noncopyable
{
	typedef boost::function<void(EventLoop*)> ThreadInitCallback;
    typedef std::map<std::string, TcpConnectionPtr> TcpConnectionMap;
public:
	TcpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string& name);
    ~TcpServer();	
    
	const std::string& getName() const { return mNameArg; }
	EventLoop* getLoop() const { return mEventLoop; }
  
	void setThreadNum(int numThreads);
    void setThreadInitCallback(const ThreadInitCallback& cb){ mThreadInitCallback = cb; }

	void start();

	/** setting callback functions */
	void setConnectionCallback(const ConnectionCallback& cb){ mConnectionCallback = cb; }
	void setMessageCallback(const MessageCallback& cb){ mMessageCallback = cb; }
    
	// for test
	int connecionSize()const {return mTcpConnection.size();}
private:
	/** 
    * Called when a connection is completed, this call is not thread safe, but in loop
    */
	void onNewConnection(int fd, const InetAddress& peerAddr);
    
	/** 
    * Called when a connection is disconnected, this call is thread safe
    */
    void onRemoveConnection(const TcpConnectionPtr& conn);
    
	/** 
    * Called when a connection is disconnected, this call is not thread safe, but in loop
    */
	void onRemoveConnectionInLoop(const TcpConnectionPtr& conn);
private:
	std::string mNameArg;
    int mNextConnId;
	EventLoop* mEventLoop;   
	bool mIsStarted;

	ConnectionCallback mConnectionCallback;
    MessageCallback mMessageCallback;
	ThreadInitCallback mThreadInitCallback;
    
	boost::shared_ptr<EventLoopThreadPool> mThreadPool;
	boost::scoped_ptr<TcpAcceptor> mTcpAcceptor; 
	TcpConnectionMap mTcpConnection;
};





}  // ~net
}  // ~moon
#endif  // ~MOON_NET_TCPSERVER_H_