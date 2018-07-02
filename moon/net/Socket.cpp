#include <moon/net/Socket.h>
#include <moon/net/SocketOps.h>

#include <netinet/tcp.h>

using namespace moon;
using namespace moon::net;


/** 析构函数 */
Socket::~Socket()
{
	sockets::close(mFd);
}


int Socket::shutdownWrite()
{
	return sockets::shutdownWrite(mFd);
}

void Socket::setTcpNoDelay(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(mFd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof optval);
}

