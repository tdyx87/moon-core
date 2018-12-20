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
	ServerLoader();
public:
	static ServerLoader& getInstance();

	int load(EventLoop *loop, const std::string &configPath);
	TcpServerPtr getServer();
private:
	TcpServerPtr mServer;

};  //~ ServerLoader

}  //~ net
}  //~ moon

#endif