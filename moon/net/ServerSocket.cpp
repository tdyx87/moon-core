/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/net/ServerSocket.h>
#include <moon/net/SocketOps.h>

#include <errno.h>
#include <string.h>

using namespace moon;
using namespace moon::net;

ServerSocket::ServerSocket(uint16_t port, bool nonblock, bool reuseAddr) : mLocalAddress(port)
{
	mSocketFd = nonblock ? sockets::createNonblockingSocket() : sockets::createSocket();
	mReuseAddr = reuseAddr;
}

ServerSocket::ServerSocket(const InetAddress& localAddress, bool nonblock, bool reuseAddr) : mLocalAddress(localAddress)
{
	mSocketFd = nonblock ? sockets::createNonblockingSocket() : sockets::createSocket();
	mReuseAddr = reuseAddr;
}

ServerSocket::~ServerSocket()
{
	sockets::close(mSocketFd);
}

int ServerSocket::listen()
{
	if (mSocketFd < 0) {
		return -1;
	}

	if (init(mReuseAddr) != 0) {
		return -1;
	}

	return sockets::listen(mSocketFd);
}

int ServerSocket::accept(InetAddress* peerAddr)
{
	struct sockaddr_in addr;
	bzero(&addr, sizeof addr);

	int connfd = sockets::accept(mSocketFd, &addr);
	if (connfd >= 0) {
	    peerAddr->setSockAddrInet(addr);
	}

	return connfd;
}

int ServerSocket::setReuseAddr(bool on)
{
	int optval = on ? 1 : 0; 
	return ::setsockopt(mSocketFd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval));
}

int ServerSocket::init(bool reuseAddr)
{
	if (reuseAddr) {
		if (this->setReuseAddr(reuseAddr) != 0) {
			return -1;
		}
	}

	if (bind() != 0) {
		return -1;
	}

	return 0;
}


int ServerSocket::bind()
{
    return sockets::bind(mSocketFd, mLocalAddress.getSockAddrInet());
}
