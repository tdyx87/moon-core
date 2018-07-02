/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_SOCKET_H_
#define MOON_NET_SOCKET_H_

#include <boost/noncopyable.hpp>

namespace moon
{
namespace net
{

/**
* Socket类对象表示一个客户端连接的Tcp连接对象
*/
class Socket : boost::noncopyable
{
public:
    explicit Socket(int fd) : mFd(fd) {}

	~Socket();

	int getFd()const {return mFd;}	

	/**
	* 关闭socket写端，调用此方法后不能再向该socket对象写入数据，
	* 但是依然可以从该socket对象读取数据
	* @return 成功返回0, 失败返回-1
	*/
	int shutdownWrite();

	void setTcpNoDelay(bool on);
	
private:
	const int mFd;
};



}  //~ namespace net
}  //~ namespace moon

#endif  // ~MOON_NET_SOCKET_H_