/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_INETADDRESS_H_
#define MOON_NET_INETADDRESS_H_

/**
* 文件名: InetAddress.h
* 文件描述: 网络地址对象
*/

#include <netinet/in.h>
#include <string>

namespace moon
{
namespace net
{

class InetAddress
{
public:
	explicit InetAddress(uint16_t port = 0);
	
	InetAddress(const char* ip, uint16_t port);

	InetAddress(const struct sockaddr_in& addr) : mSockaddr(addr) { }

	/** 获取套接字地址结构 */
	const struct sockaddr_in& getSockAddrInet()const {return mSockaddr;}
    
	/** 获取地址中的IP地址 */
	std::string toIp() const;

	std::string toIpPort() const;
    
	/** 获取地址中的端口 */
	uint16_t toPort() const;
    
	/** 设置套接字地址结构 */
	void setSockAddrInet(const struct sockaddr_in& addr) { mSockaddr = addr; }

private:
    struct sockaddr_in mSockaddr;
};  // ~CInetAddress


}  //~ namespace net
}  //~ namespace moon

#endif  // ~MOON_NET_INET_ADDRESS_H_