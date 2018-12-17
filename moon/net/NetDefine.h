/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_NET_NETDEFINE_H_
#define MOON_NET_NETDEFINE_H_

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace moon
{

namespace net
{
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

class Buffer;
class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void (const TcpConnectionPtr&, Buffer&)> MessageCallback;

class Connection;
typedef std::shared_ptr<Connection> ConnectionPtr;
typedef std::map<std::string, ConnectionPtr> ConnectionMap;
typedef std::function<void (const ConnectionPtr&)> ConnectionCallback1;
typedef std::function<void (const ConnectionPtr&)> CloseCallback1;
typedef std::function<void (const ConnectionPtr&, Buffer&)> MessageCallback1;

typedef std::function<void (int sockfd)> NewConnectionCallback;




}  // ~net
}  // ~moon
#endif  // ~MOON_NET_NETDEFINE_H_