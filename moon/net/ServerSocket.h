/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_SERVERSOCKET_H_
#define MOON_NET_SERVERSOCKET_H_

#include <moon/net/InetAddress.h>

#include <boost/noncopyable.hpp>

namespace moon
{
namespace net
{

class InetAddress;

/**
* 一个ServerSocket对象代表一个被监听的socket，不同于@Socket
*/
class ServerSocket : boost::noncopyable
{
public:
    ServerSocket(uint16_t port, bool nonblock = true, bool reuseAddr = true);
    ServerSocket(const InetAddress& localAddress, bool nonblock = true, bool reuseAddr = true);

	~ServerSocket();

	int getFd()const {return mSocketFd;}	
   
	int listen();
    
	/** 
	@description:
	    Accept a connection on a socket.
	@param: pobjPeerAddr
	    input and output parameter, which will be changed by accept
	@return: 
	    On success, returns a non-negaive integer that is a descriptor for the accepted socket,
    which has been set to non-block and close-on-exec, *pobjPeerAddr is assigned.
	    On error, -1 is returned, and *pobjPeerAddr is untouched.
	*/
	int accept(InetAddress* peerAddr);
    
    int setReuseAddr(bool on);
private:
	int init(bool reuseAddr);
	int bind();	
private:
	int mSocketFd;
    InetAddress mLocalAddress;
	bool mReuseAddr;
};




}  //~ namespace net
}  //~ namespace moon

#endif  // ~MOON_NET_SERVER_SOCKET_H_