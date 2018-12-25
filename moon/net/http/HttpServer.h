#ifndef MOON_NET_HTTP_HTTPSERVER_H_
#define MOON_NET_HTTP_HTTPSERVER_H_

#include <moon/net/NetDefine.h>

namespace moon
{
namespace net
{
class HttpRequest;
class HttpServer
{
public:
	typedef std::function<void (const HttpRequest&, TcpConnectionPtr conn)> HttpCallback;

	HttpServer(const TcpServerPtr &server);
	~HttpServer();

	void setHttpCallback(const HttpCallback &cb)
    {
      mHttpCallback = cb;
    }

    void start();
private:
	void onConnection(const TcpConnectionPtr &conn, bool connected);
    void onMessage(const TcpConnectionPtr &conn, Buffer &buf);
    void onRequest(const TcpConnectionPtr &conn, const HttpRequest &request);
private:
	TcpServerPtr mServer;
	HttpCallback mHttpCallback;
};  // ~HttpServer

}  // ~net
}  // ~moon
#endif