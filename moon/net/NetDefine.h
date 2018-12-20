/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_NET_NETDEFINE_H_
#define MOON_NET_NETDEFINE_H_

#include <moon/Slice.h>
#include <functional>
#include <memory>

namespace moon
{

namespace net
{
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

class Buffer;
class TcpConnection;
class TcpServer;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::shared_ptr<TcpServer> TcpServerPtr;

typedef std::function<void (const TcpConnectionPtr&, bool)> OnConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&)> OnCloseCallback;
typedef std::function<void (const TcpConnectionPtr&, Buffer&)> OnMessageCallback;

typedef std::function<void (int sockfd)> OnNewConnectionCallback;




}  // ~net
}  // ~moon
#endif  // ~MOON_NET_NETDEFINE_H_