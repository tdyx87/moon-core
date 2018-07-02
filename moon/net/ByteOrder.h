/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_BYTEORDER_H_
#define MOON_NET_BYTEORDER_H_

#include <netinet/in.h>

namespace moon
{
namespace net
{
namespace sockets
{

/** 主机字节序转网络字节序(16位) */
inline uint16_t hostToNetwork16(uint16_t host16)
{
	return htons(host16);
}

/** 主机字节序转网络字节序(32位) */
inline uint32_t hostToNetwork32(uint32_t host32)
{
	return htonl(host32);
}


/** 网络字节序转主机字节序(16位) */
inline uint16_t networkToHost16(uint16_t net16)
{
	return ntohs(net16);
}

/** 网络字节序转主机字节序(32位) */
inline uint32_t networkToHost32(uint32_t net32)
{
	return ntohl(net32);
}


}  // ~namespace sockets
}  // ~namespace net
}  // ~namespace moon

#endif  // ~MOON_NET_BYTE_ORDER_H_