/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/net/ByteOrder.h>
#include <moon/net/SocketOps.h>
#include <moon/TypeCast.h>

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace moon;
using namespace moon::net;

/** 地址转换 (sockaddr_in to sockaddr) */
const struct sockaddr* sockets::sockaddr_cast(const struct sockaddr_in* addr)
{
	return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

/** 地址转换 (sockaddr_in to sockaddr) */
struct sockaddr* sockets::sockaddr_cast(struct sockaddr_in* addr)
{
	return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}

/** 地址转换 (sockaddr to sockaddr_in) */
const struct sockaddr_in* sockets::sockaddr_in_cast(const struct sockaddr* addr)
{
	return static_cast<const struct sockaddr_in*>(implicit_cast<const void*>(addr));
}

/** 地址转换 (sockaddr to sockaddr_in) */
struct sockaddr_in* sockets::sockaddr_in_cast(struct sockaddr* addr)
{
	return static_cast<struct sockaddr_in*>(implicit_cast<void*>(addr));
}

/** 创建套接字 */
int sockets::createSocket()
{
	return ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
}


/** 创建非阻塞的套接字 */
int sockets::createNonblockingSocket()
{
	int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd >= 0) {
	    setNonBlockAndCloseOnExec(sockfd);
	}

	return sockfd;
}


/** 将一个本地地址绑定到该套接字上 */
int sockets::bind(int sockfd, const struct sockaddr_in& addr)
{
    return ::bind(sockfd, reinterpret_cast<struct sockaddr *>(const_cast<struct sockaddr_in *>(&addr)), static_cast<socklen_t>(sizeof addr));
}

/** 设置套接字为非阻塞 */
int sockets::setNonBlock(int sockfd)
{
	// non-block
	int flags = ::fcntl(sockfd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	return ::fcntl(sockfd, F_SETFL, flags);
}


/** 设置套接字为非阻塞,并且在exec时自动关闭 */
int sockets::setNonBlockAndCloseOnExec(int sockfd)
{
	// non-block
	int flags = ::fcntl(sockfd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	::fcntl(sockfd, F_SETFL, flags);
	// FIXME check

	// close-on-exec
	flags = ::fcntl(sockfd, F_GETFD, 0);
	flags |= FD_CLOEXEC;
	return ::fcntl(sockfd, F_SETFD, flags);
}


/** 监听套接字 */
int sockets::listen(int sockfd)
{
	return ::listen(sockfd, SOMAXCONN);
}


/** 接受套接字上的连接请求 */
int sockets::accept(int sockfd, struct sockaddr_in* addr)
{
	socklen_t addrlen = static_cast<socklen_t>(sizeof (*addr));

#if VALGRIND || defined (NO_ACCEPT4)
	int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
	setNonBlockAndCloseOnExec(connfd);
#else
	int connfd = ::accept4(sockfd, sockaddr_cast(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif

	return connfd;
}

/**  */
int sockets::connect(int sockfd, const struct sockaddr_in& addr)
{
    return ::connect(sockfd, sockaddr_cast(&addr), static_cast<socklen_t>(sizeof addr));
}


/** 关闭套接字 */
int sockets::close(int sockfd)
{
	return ::close(sockfd);
}

/** 关闭写端 */
int sockets::shutdownWrite(int sockfd)
{
	return ::shutdown(sockfd, SHUT_WR);	
}

/** 从fd读数据 */
ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
	return ::read(sockfd, buf, count);
}

/** 从fd读数据 */
ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
    return ::readv(sockfd, iov, iovcnt);
}

/** 向fd写数据 */
ssize_t sockets::write(int sockfd, const void *buf, size_t count)
{
	return ::write(sockfd, buf, count);
}


/** 获取地址sockaddr_in的IP+PORT信息转(IP:port的形式) */
void sockets::toIpPort(char* buf, size_t size, const struct sockaddr_in& addr)
{
	if ( (buf == NULL) || (size <= 0) ) {
		return ;
	}

	::inet_ntop(AF_INET, &addr.sin_addr, buf, static_cast<socklen_t>(size));
	size_t end = ::strlen(buf);
	uint16_t port = networkToHost16(addr.sin_port);
	snprintf(buf+end, size-end, ":%u", port);
}

/** 获取地址sockaddr_in的IP信息 */
void sockets::toIp(char* buf, size_t size, const struct sockaddr_in& addr)
{
	if ( (buf == NULL) || (size <= 0) ) {
		return ;
	}

	::inet_ntop(AF_INET, &addr.sin_addr, buf, static_cast<socklen_t>(size));
}

int sockets::getSocketError(int sockfd)
{
	int optval;
	socklen_t optlen = static_cast<socklen_t>(sizeof optval);

	if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) {
	    return errno;
	} else {
	    return optval;
	}
}

struct sockaddr_in sockets::getLocalAddr(int sockfd)
{
	struct sockaddr_in localaddr;
	bzero(&localaddr, sizeof localaddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
	if (::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0 ) {
		// TODO
	}
	return localaddr;
}

struct sockaddr_in sockets::getPeerAddr(int sockfd)
{
	struct sockaddr_in peeraddr;
	bzero(&peeraddr, sizeof peeraddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
	if (::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen) < 0) {	
		// TODO
	}
	return peeraddr;
}

bool sockets::isSelfConnect(int sockfd)
{
	struct sockaddr_in localaddr = getLocalAddr(sockfd);
	struct sockaddr_in peeraddr = getPeerAddr(sockfd);
	return localaddr.sin_port == peeraddr.sin_port && localaddr.sin_addr.s_addr == peeraddr.sin_addr.s_addr;
}
