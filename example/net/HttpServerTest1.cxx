#include <moon/net/http/HttpServer.h>
#include <moon/net/http/HttpRequest.h>
#include <moon/net/http/HttpResponse.h>
#include <moon/net/ServerLoader.h>
#include <moon/net/TcpConnection.h>
#include <moon/net/TcpServer.h>
#include <moon/net/NetDefine.h>
#include <moon/os/EventLoop.h>
#include <moon/Logger.h>
#include <moon/Number.h>

using std::string;
using namespace moon;
using namespace moon::net;
static const std::string gConfigPath = "/mnt/hgfs/Linux/moon/moon/example/conf/http_server.cfg";
static int gIndex = 0;

EventLoop gEventLoop;
HttpServer *gHttpServer;

static void onHttpRequest(const HttpRequest &request, const TcpConnectionPtr &conn)
{
	LOGGER_TRACE("path:%s query:%s", request.getPath().c_str(), request.getQuery().c_str());
	std::string reply = HttpResponse::getDefaultResponse("reply-" + Number::intToString(++gIndex));
    conn->send(reply.c_str());
}

static void startServer()
{
    if (ServerLoader::getInstance().load(&gEventLoop, gConfigPath) != 0) {
    	LOGGER_ERROR("load server failed");
        return ;
    }

    gHttpServer = new HttpServer(ServerLoader::getInstance().getServer());
    gHttpServer->setHttpCallback(std::bind(&onHttpRequest, _1, _2));
    gHttpServer->start();
    gEventLoop.loop();
}

int main(int argc, char* argv[])
{  	
	startServer();

	return 0;
}

//http://192.168.8.132:10003?helloworld
