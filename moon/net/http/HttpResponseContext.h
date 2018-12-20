#ifndef MOON_NET_HTTP_HTTPRESPONSECONTEXT_H_
#define MOON_NET_HTTP_HTTPRESPONSECONTEXT_H_

#include <moon/net/http/HttpResponse.h>

namespace moon
{
namespace net
{

class Buffer;
class HttpResponseContext
{
public:
	enum HttpResponseParseState
	{
	    kExpectStatusLine,
        kExpectHeaders,
        kExpectBody,
        kGotAll
	};  //~ HttpRequestParseState

	HttpResponseContext(Buffer *buffer) : mState(kExpectStatusLine), mBuffer(buffer)
    {
    }

	bool isGotAll()const{return kGotAll == mState;}

    void reset()
    {
        mState = kExpectStatusLine;
        HttpResponse dummy;
        mResponse.swap(dummy);
    }

    const HttpResponse& getResponse() const
    {
    	return mResponse;
    }

    HttpResponse& getResponse()
    {
    	return mResponse;
    }

    // 解析HTTP请求
    bool parseResponse();
private:
	// 解析状态行
	bool parseStatusLine();
	// 解析消息报头
	bool parseHeader();
	// 解析请求正文
	bool parseBody();
private:
	HttpResponseParseState mState;
    HttpResponse mResponse;
	Buffer *mBuffer;
};



}  //~ namespace net
}  //~ namespace moon
#endif  //~ MOON_NET_HTTP_HTTP_RESPONSE_CONTEXT_H_
