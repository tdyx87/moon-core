/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_SERVER1_H_
#define MOON_NET_SERVER1_H_

#include <moon/noncopyable.h>
#include <moon/net/NetDefine.h>

#include <string>
#include <map>

namespace moon
{
class EventLoop;

namespace net
{
class InetAddress;
class IOThread;
class TcpAcceptor;

class Server1 : noncopyable
{
public:
	Server1(EventLoop* loop, const InetAddress& listenAddr, const std::string& name);
    ~Server1();	
    
	const std::string& getName() const { return mNameArg; }
	EventLoop* getLoop() const { return mEventLoop; }
  
	void setThreadNum(int numThreads);
	void start();

	/** setting callback functions */
	void setConnectionCallback(const ConnectionCallback1& cb){ mConnectionCallback = cb; }
	void setMessageCallback(const MessageCallback1& cb){ mMessageCallback = cb; }
private:
	/** 
    * Called when a connection is completed, this call is not thread safe, but in loop
    */
	void onNewConnection(int fd, const InetAddress& peerAddr);
private:
	std::string mNameArg;
    int mNextConnId;
	EventLoop* mEventLoop;   
	bool mIsStarted;

	ConnectionCallback1 mConnectionCallback;
    MessageCallback1 mMessageCallback;
    
	std::unique_ptr<IOThread> mIOThread;
	std::unique_ptr<TcpAcceptor> mTcpAcceptor;
};





}  // ~net
}  // ~moon
#endif  // ~MOON_NET_TCPSERVER_H_