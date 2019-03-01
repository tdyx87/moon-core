#ifndef MOON_NET_SERVERLOADER_H_
#define MOON_NET_SERVERLOADER_H_

#include <moon/net/NetDefine.h>
#include <string>

namespace moon
{
class EventLoop;

namespace net
{
class ServerLoader
{
public:
	static TcpServerPtr load(EventLoop *loop, const std::string &configPath);

};  //~ ServerLoader

}  //~ net
}  //~ moon

#endif