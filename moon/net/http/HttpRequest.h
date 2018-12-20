#ifndef MOON_NET_HTTP_HTTPREQUEST_H_
#define MOON_NET_HTTP_HTTPREQUEST_H_

#include <string>
#include <map>

namespace moon
{
namespace net
{

using std::string;
using std::map;

class HttpRequest
{
public:
	// http 请求方法定义
	enum Method{kInvalid, kGet, kPost, kHead, kPut, kDelete};
	// http版本定义
	enum Version{kUnknown, kHttp10, kHttp11};

	HttpRequest() : mMethod(kInvalid), mVersion(kUnknown)
	{
	}

	bool parseRequest(const std::string& request);

	void setVersion(Version v)
	{
		mVersion = v;
	}

	Version getVersion() const
	{
		return mVersion;
	}

	bool setMethod(const std::string& m);

	Method getMethod() const
	{
		return mMethod;
	}

	std::string methodString() const;
	

    void setPath(const std::string& requestPath)
    {
    	mRequestPath = requestPath;
    }

	const std::string& getPath() const
	{
		return mRequestPath;
	}

    void setQuery(const std::string& query)
    {
    	mQuery = query;
    }

	const std::string& getQuery() const
	{
		return mQuery;
	}

	void setBody(const std::string& body)
	{
		mBody = body;
	}

	const std::string& getBody() const
	{
		return mBody;
	}

	void addHeader(const std::string& field, const std::string& value);	

	std::string getHeader(const std::string& field) const;

	const std::map<string, string>& headers() const
	{
		return mHttpHeaders;
    }

	void swap(HttpRequest& that);    

private:
	Method mMethod;
    Version mVersion;
	std::string mRequestPath;  // http请求路径
	std::string mQuery;
	std::string mBody;
	std::map<std::string, std::string> mHttpHeaders;  // http请求头
};



}  //~ namespace net
}  //~ namespace moon
#endif  //~ MOON_NET_HTTP_HTTPREQUEST_H_
