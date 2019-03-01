#include <moon/net/http/HttpServer.h>
#include <moon/net/http/HttpRequest.h>
#include <moon/net/http/HttpResponse.h>
#include <moon/net/ServerLoader.h>
#include <moon/net/TcpConnection.h>
#include <moon/net/TcpServer.h>
#include <moon/net/NetDefine.h>
#include <moon/core/EventLoop.h>
#include <moon/Logger.h>
#include <moon/Number.h>

using std::string;
using namespace moon;
using namespace moon::net;
static const std::string gConfigPath1 = "/mnt/hgfs/linux/moon/moon/example/conf/http_server1.cfg";
static const std::string gConfigPath2 = "/mnt/hgfs/linux/moon/moon/example/conf/http_server2.cfg";
static int gIndex = 0;

EventLoop gEventLoop;
HttpServer *gHttpServer;

static void printHeaders(const std::map<string, string> &headers) 
{
    std::map<string, string>::const_iterator it = headers.cbegin();
    for (; it != headers.cend(); ++it) { 
        LOGGER_TRACE("[%s]:%s", it->first.c_str(), it->second.c_str());
    }
}

static void onHttpRequest(const HttpRequest &request, const TcpConnectionPtr &conn)
{
	LOGGER_TRACE("fd[%d] path:%s query:%s", conn->getFd(), request.getPath().c_str(), request.getQuery().c_str());
    const std::map<string, string> &headers = request.headers();
    printHeaders(headers);

	std::string reply = HttpResponse::getDefaultResponse("reply-" + Number::intToString(++gIndex));
    conn->send(reply.c_str());
}

static void startServer(const std::string &configPath)
{
    TcpServerPtr server = ServerLoader::load(&gEventLoop, configPath);
    if (!server) {
    	LOGGER_ERROR("load server failed");
        return ;
    }

    gHttpServer = new HttpServer(server);
    gHttpServer->setHttpCallback(std::bind(&onHttpRequest, _1, _2));
    gHttpServer->start();
    gEventLoop.loop();
}

int main(int argc, char* argv[])
{  	
    if (argc < 2) {
        LOGGER_ERROR("need more argument");
        return -1;
    }
    if (argv[1][0] == '1') {
        startServer(gConfigPath1);
    } else {
        startServer(gConfigPath2);
    }

	return 0;
}

//http://192.168.8.132:10003?helloworld
// http://192.168.23.241/test/api/markets?name=vsgoster-1
