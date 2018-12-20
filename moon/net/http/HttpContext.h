#ifndef MOON_NET_HTTP_HTTPCONTEXT_H_
#define MOON_NET_HTTP_HTTPCONTEXT_H_

#include <moon/net/http/HttpRequest.h>

namespace moon
{
namespace net
{

class Buffer;
class HttpContext
{
public:
	enum HttpRequestParseState
	{
	    kExpectRequestLine,
        kExpectHeaders,
        kExpectBody,
        kGotAll
	};  //~ HttpRequestParseState

    HttpContext(Buffer *buffer) : mState(kExpectRequestLine), mBuffer(buffer)
    {
    }

	bool isGotAll()const{return kGotAll == mState;}

    void reset()
    {
        mState = kExpectRequestLine;
        HttpRequest dummy;
        mRequest.swap(dummy);
    }

    const HttpRequest& request() const { return mRequest;}

    HttpRequest& request() { return mRequest;} 

    // 解析HTTP请求
    bool parseRequest();
private:
	// 解析请求行
	bool parseRequestLine();
	// 解析消息报头
	bool parseHeader();
	// 解析请求正文
	bool parseBody();
private:
    HttpRequestParseState mState;
    HttpRequest mRequest;
	Buffer *mBuffer;
};



}  //~ namespace net
}  //~ namespace moon
#endif  //~ MOON_NET_HTTP_HTTP_CONTEXT_H_
