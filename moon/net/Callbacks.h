/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_NET_CALLBACKS_H_
#define MOON_NET_CALLBACKS_H_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace moon
{
namespace net
{

class Buffer;
class TcpConnection;

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef boost::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef boost::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef boost::function<void (const TcpConnectionPtr&, Buffer&)> MessageCallback;

typedef boost::function<void (int sockfd)> NewConnectionCallback;




}  // ~net
}  // ~moon
#endif  // ~MOON_NET_CALLBACKS_H_