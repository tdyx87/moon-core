#include <moon/net/InetAddress.h>
#include <moon/net/TcpServer.h>
#include <moon/net/TcpConnection.h>
#include <moon/logger/Logger.h>
#include <moon/core/EventLoop.h>
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

std::string g_listenPortString;
EventLoop g_objEventLoop;
TcpServer *g_pobjTcpServer;

int iResponse = 0;

static std::string packHttpResp(char* data)
{
	return std::string(data);
/*	if ((NULL == data))
	{
		return "";
	}
	
	char httpResp[1024] = {0};
	sprintf(httpResp, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %lu\r\n\r\n%s\r\n",
		    strlen(data), data);
	
	return std::string(httpResp);
*/
}

static string getResponse()
{
	++iResponse;
	char buf[64] = {0};
	snprintf(buf, sizeof(buf), "response data:%d", iResponse);
	return packHttpResp(buf);
}

void startCallback(EventLoop* loop)
{
	LOGGER_INFO("Loop线程启动成功");
}
void onClientConnection(const TcpConnectionPtr& clientConn, bool connected)
{
	LOGGER_TRACE("连接:fd[%d] %s %s -> %s %s", clientConn->getFd(), clientConn->getName().c_str(), 
		clientConn->getPeerAddress().toIpPort().c_str(), clientConn->getLocalAddress().toIpPort().c_str(),
		connected ? "已连接":"已断开");
}

void onClientMessage(const TcpConnectionPtr& conn, Buffer &buffer)
{
	string data = buffer.retrieveAllAsString();
	LOGGER_DEBUG("连接:%s 接受数据:%s", conn->getName().c_str(), data.c_str());
    
	string sendData = getResponse();
	conn->send(sendData.c_str(), sendData.length());
	//conn->shutdown();
	LOGGER_TRACE("response:%s", sendData.c_str());
}

static void startServer1()
{
	uint16_t listenPort = static_cast<uint16_t>(Number::parseInt(g_listenPortString));    
	InetAddress objInetAddress(listenPort);
	g_pobjTcpServer = new TcpServer(&g_objEventLoop, objInetAddress, "test");
	g_pobjTcpServer->setThreadNum(2);
	g_pobjTcpServer->setThreadInitCallback(std::bind(startCallback, _1));
	//g_pobjTcpServer->setConnectionCallback(std::bind(onClientConnection, _1, _2));
	g_pobjTcpServer->setMessageCallback(std::bind(onClientMessage, _1, _2));
    
	g_pobjTcpServer->start();
	g_objEventLoop.loop();	

}
//static_assert(sizeof(void *) == 4, "64-bit code generation is not supported.");

class Test
{
public:
    void TestWork(int index)
    {
    	printf("TestWork 1\n");
    }
    void TestWork(int * index)
    {
       printf("TestWork 2\n");
    }
};

int test4()
{
    Test test;
    //test.TestWork(NULL);
    test.TestWork(nullptr);
    return 0;
}

int main(int argc, char* argv[])
{  
	if (NULL == argv[1])
	{
		LOGGER_ERROR("请提供argv[1]");
		return -1;
	}
	g_listenPortString = string(argv[1]);
	LOGGER_TRACE("监听端口:%s", argv[1]);
	startServer1();

	return 0;
}

//http://192.168.8.132:10003?helloworld
