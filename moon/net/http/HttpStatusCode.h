#ifndef MOON_NET_HTTP_HTTPSTATUSCODE_H_
#define MOON_NET_HTTP_HTTPSTATUSCODE_H_

#include <string>

namespace moon
{
namespace net
{
namespace HttpStatusCode
{
using std::string;

static const int k200Ok = 200;  // 成功
static const int k400BadRequest = 400;
static const int k404NotFound = 404;

static const int k500InternalServerError = 500;
static const int k505HttpVersionNotSupported = 505;

static string getCodeInfo(int code)
{
    switch (code)
    {
    case k200Ok:
    	return "OK";
    case k400BadRequest:
    	return "Bad Request";
    case k404NotFound:
    	return "Not Found";

    case k500InternalServerError:
        	return "Internal Server Error";
    case k505HttpVersionNotSupported:
            return "Http Version Not Supported";
    default:
    	return "Unknown";
    }

    return "Unknown";
}  //~ getCodeInfo()



}  //~ HttpStatusCode
}  //~ net
}  //~ moon
#endif  //~ MOON_NET_HTTP_HTTPSTATUSCODE_H_
