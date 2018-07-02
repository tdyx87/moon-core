/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_SOCKETOPS_H_
#define MOON_NET_SOCKETOPS_H_

#include <arpa/inet.h>

namespace moon
{
namespace net
{
namespace sockets
{

/** 地址转换 (sockaddr_in to sockaddr) */
const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);

/** 地址转换 (sockaddr_in to sockaddr) */
struct sockaddr* sockaddr_cast(struct sockaddr_in* addr);

/** 地址转换 (sockaddr to sockaddr_in) */
const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);

/** 地址转换 (sockaddr to sockaddr_in) */
struct sockaddr_in* sockaddr_in_cast(struct sockaddr* addr);


/** 创建套接字(阻塞模式) */
int createSocket();


/** 创建非阻塞的套接字 */
int createNonblockingSocket();


/** 将一个本地地址绑定到该套接字上 */
int bind(int sockfd, const struct sockaddr_in& addr);


/** 设置套接字为非阻塞 */
int setNonBlock(int sockfd);


/** 设置套接字为非阻塞,并且在exec时自动关闭 */
int setNonBlockAndCloseOnExec(int sockfd);


/** 监听套接字 */
int listen(int sockfd);


/** 接受套接字上的连接请求 */
int accept(int sockfd, struct sockaddr_in* addr);


/**  */
int connect(int sockfd, const struct sockaddr_in& addr);


/** 关闭套接字 */
int close(int sockfd);


/** 关闭写端 */
int shutdownWrite(int sockfd);

/** 从fd读数据 */
ssize_t read(int sockfd, void *buf, size_t count);


/** 从fd读数据 */
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);


/** 向fd写数据 */
ssize_t write(int sockfd, const void *buf, size_t count);

	
/** 获取地址sockaddr_in的IP+PORT信息转(IP:port的形式) */
void toIpPort(char* buf, size_t size, const struct sockaddr_in& addr);


/** 获取地址sockaddr_in的IP信息 */
void toIp(char* buf, size_t size, const struct sockaddr_in& addr);

int getSocketError(int sockfd);

struct sockaddr_in getLocalAddr(int sockfd);
struct sockaddr_in getPeerAddr(int sockfd);
bool isSelfConnect(int sockfd);


}  // ~sockets
}  // ~net
}  // ~moon

#endif  // ~MOON_NET_SOCKET_OPS_H_