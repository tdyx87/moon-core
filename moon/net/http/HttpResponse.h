#ifndef MOON_NET_HTTP_HTTPRESPONSE_H_
#define MOON_NET_HTTP_HTTPRESPONSE_H_

#include <moon/net/Buffer.h>
#include <string>
#include <map>

namespace moon
{
namespace net
{
using std::string;
using std::map;

class HttpResponse
{
public:
/*	enum HttpStatusCode
	{
	    kUnknown, 
	    k200Ok = 200,
        k400BadRequest = 400,
        k404NotFound = 404,
	};  //~ HttpStatusCode

	void setStatusCode(HttpStatusCode code)
	{
		std::string message;
		mStatusCode = code;
		switch (code)
		{
		case kUnknown:
			message = "Unknown";
			break;
		case k200Ok:
			message = "OK";
			break;
		case k400BadRequest:
			message = "Bad Request";
			break;
		case k404NotFound:
			message = "Not Found";
			break;
	    default:
			message = "Unknown";
			break;
		
		}
		mStatusMessage = message;
	}
*/
	void setStatusCode(int code)
	{
		mStatusCode = code;
	}

	int getStatusCode()const
	{
		return mStatusCode;
	}

	void setContentType(const string& contentType)
	{
		addHeader("Content-Type", contentType);
	}
	
    void addHeader(const string& key, const string& value)
    { 
		mHttpHeaders[key] = value; 
	}

    std::string getHeader(const std::string& field) const;

    void setBody(const string& body)
    { 
		mHttpBody = body; 
	}

    void swap(HttpResponse& that);
    std::string toString();

	static std::string getDefaultResponse(const std::string& body);
	static std::string getErrorDefaultResponse(int code);
private:
    std::map<std::string, std::string> mHttpHeaders;  // http头
    int mStatusCode;
	std::string mHttpBody;  // HTTP响应体
};

}  // ~net
}  // ~moon
#endif  //~ MOON_NET_HTTP_HTTPRESPONSE_H_
