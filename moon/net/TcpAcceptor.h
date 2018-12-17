/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_TCPACCEPTOR_H_
#define MOON_NET_TCPACCEPTOR_H_

#include <moon/noncopyable.h>
#include <moon/net/ServerSocket.h>
#include <moon/os/EventChannel.h>

#include <functional>

namespace moon
{
class EventLoop;

namespace net
{
class InetAddress;

class TcpAcceptor : noncopyable
{
public:
	typedef std::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

	TcpAcceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
    ~TcpAcceptor();
    
	/** 设置新连接回调接口 */
	void setNewConnectionCallback(const NewConnectionCallback& cb) { mNewConnectionCallback = cb; }
    
	/** 是否处于监听状态 */
	bool listenning() const { return mIsListenning; }

	int listen();
private:
	void handleRead();
private:
	EventLoop *mEventLoop;
	ServerSocket mServerSocket;
	EventChannel mAcceptChannel;	

    NewConnectionCallback mNewConnectionCallback;
	bool mIsListenning;
};



}  // ~net
}  // ~moon
#endif  // ~MOON_NET_TCP_ACCEPTOR_H_