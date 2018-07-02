#include <moon/net/InetAddress.h>
#include <moon/net/ByteOrder.h>
#include <moon/net/SocketOps.h>

#include <arpa/inet.h>
#include <strings.h>

using namespace moon;
using namespace moon::net;

/** 构造函数 */
InetAddress::InetAddress(uint16_t port)
{
	bzero(&mSockaddr, sizeof(mSockaddr));

	mSockaddr.sin_family = AF_INET;
	mSockaddr.sin_addr.s_addr = sockets::hostToNetwork32(INADDR_ANY);
	mSockaddr.sin_port = sockets::hostToNetwork16(port);
}

/** 构造函数 */
InetAddress::InetAddress(const char* ip, uint16_t port)
{
	bzero(&mSockaddr, sizeof(mSockaddr));

	mSockaddr.sin_family = AF_INET;
	mSockaddr.sin_port = sockets::hostToNetwork16(port);

	inet_pton(AF_INET, ip, &mSockaddr.sin_addr.s_addr);
}


/** 获取地址中的IP地址 */
std::string InetAddress::toIp()const
{
	char buf[32];
    sockets::toIp(buf, sizeof buf, mSockaddr);
    return buf;	
}

std::string InetAddress::toIpPort() const
{
	char buf[32];
	sockets::toIpPort(buf, sizeof buf, mSockaddr);
	return buf;
}

/** 获取地址中的端口 */
uint16_t InetAddress::toPort()const
{	
	return sockets::networkToHost16(mSockaddr.sin_port);
}
