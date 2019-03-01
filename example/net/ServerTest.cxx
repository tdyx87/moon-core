#include <moon/net/InetAddress.h>
#include <moon/net/Server.h>
#include <moon/net/TcpConnection.h>
#include <moon/logger/Logger.h>
#include <moon/core/EventLoop.h>
#include <moon/core/ServiceConfig.h>
#include <moon/Number.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <functional>
#include <memory>
#include <string>

using std::string;
using namespace moon;
using namespace moon::net;

static Server *gServer = NULL;

int iResponse = 0;

void onClientConnection(const TcpConnectionPtr& clientConn)
{
	LOGGER_TRACE("connection: fd[%d] %s %s -> %s %s", clientConn->getFd(), clientConn->getName().c_str(), 
		clientConn->getPeerAddress().toIpPort().c_str(), clientConn->getLocalAddress().toIpPort().c_str(),
		clientConn->isConnected() ? " connected" : "disconnected");
}

void onClientMessage(const TcpConnectionPtr& conn, Buffer &buf)
{
	string data = buf.retrieveAllAsString();
	LOGGER_DEBUG("connection:%s data:%s", conn->getName().c_str(), data.c_str());
    
	string sendData = data;
	conn->send(sendData.c_str(), sendData.length());
	//conn->shutdown();
	LOGGER_TRACE("response:%s", sendData.c_str());
}

static void startServer()
{
    ServiceConfig config;
    if (!ServiceConfig::loadConfig("/mnt/hgfs/Linux/moon/moon/example/net/Server.cfg", config)) {
    	LOGGER_ERROR("load config failed");
        return ;
    }

	EventLoop loop;
	gServer = new Server(&loop);
	gServer->setConnectionCallback(std::bind(onClientConnection, _1));
	gServer->setMessageCallback(std::bind(onClientMessage, _1, _2));
	gServer->start(config);

	loop.loop();	
}

int main(int argc, char* argv[])
{  
	startServer();

    LOGGER_DEBUG("------main end------");
	return 0;
}

//http://192.168.8.132:10003?helloworld
