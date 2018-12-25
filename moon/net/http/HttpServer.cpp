#include <moon/net/http/HttpServer.h>
#include <moon/net/http/HttpContext.h>
#include <moon/net/http/HttpRequest.h>
#include <moon/net/TcpConnection.h>
#include <moon/net/TcpServer.h>
#include <moon/Logger.h>

using namespace moon;
using namespace moon::net;

HttpServer::HttpServer(const TcpServerPtr &server)
{
	mServer = server;
	mServer->setConnectionCallback(std::bind(&HttpServer::onConnection, this, _1, _2));
    mServer->setMessageCallback(std::bind(&HttpServer::onMessage, this, _1, _2));
}

HttpServer::~HttpServer()
{
}

void HttpServer::start()
{
    mServer->start();
}

void HttpServer::onConnection(const TcpConnectionPtr &conn, bool connected)
{
    if (connected) {
        conn->setContext(HttpContext(&conn->inputBuffer()));
    }
}

void HttpServer::onMessage(const TcpConnectionPtr &conn, Buffer &buf)
{
    HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());

    if (!context->parseRequest())
    {
		std::string str = "HTTP/1.1 400 Bad Request\r\n\r\n";
        conn->send(str.c_str(), str.length());
        conn->shutdown();
    } 

    if (context->isGotAll())
    {
        onRequest(conn, context->request());
        context->reset();
    }
}

void HttpServer::onRequest(const TcpConnectionPtr &conn, const HttpRequest &request)
{
    mHttpCallback(request, conn);
}