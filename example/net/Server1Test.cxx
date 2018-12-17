#include <moon/net/InetAddress.h>
#include <moon/net/Server1.h>
#include <moon/net/Connection.h>
#include <moon/Logger.h>
#include <moon/os/EventLoop.h>
#include <moon/os/ServiceConfig.h>
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

static Server1 *gServer = NULL;
EventLoop gEventLoop;

int iResponse = 0;

void onClientConnection(const ConnectionPtr& clientConn)
{
	LOGGER_TRACE("connection: fd[%d] %s %s -> %s %s", clientConn->getFd(), clientConn->getName().c_str(), 
		clientConn->getPeerAddress().toIpPort().c_str(), clientConn->getLocalAddress().toIpPort().c_str(),
		clientConn->isConnected() ? " connected" : "disconnected");
}

void onClientMessage(const ConnectionPtr& conn, Buffer &buf)
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
	uint16_t listenPort = 9090;    
	InetAddress objInetAddress(listenPort);
	gServer = new Server1(&gEventLoop, objInetAddress, "test");
	//gServer->setThreadInitCallback(std::bind(startCallback, _1));
	gServer->setConnectionCallback(std::bind(onClientConnection, _1));
	gServer->setMessageCallback(std::bind(onClientMessage, _1, _2));
    
	gServer->start();
	gEventLoop.loop();	
}

int main(int argc, char* argv[])
{  
	startServer();

    LOGGER_DEBUG("------main end------");
	return 0;
}

//http://192.168.8.132:10003?helloworld
