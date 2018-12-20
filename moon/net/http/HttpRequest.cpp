#include <moon/net/http/HttpRequest.h>
#include <moon/StringUtils.h>

using namespace moon;
using namespace moon::net;

bool HttpRequest::setMethod(const std::string& m)
{
	if (m == "GET") {
		mMethod = kGet;
	} else if (m == "POST") {
		mMethod = kPost;
	} else if (m == "HEAD") {
		mMethod = kHead;
	} else if (m == "PUT") {
		mMethod = kPut;
	} else if (m == "DELETE") {
		mMethod = kDelete;
	} else {
		mMethod = kInvalid;
	}

	return mMethod != kInvalid;
}

std::string HttpRequest::methodString() const
{
	switch (mMethod)
	{
	case kGet:
		return std::string("GET");
		break;
	case kPost:
		return std::string("POST");
		break;
	case kHead:
		return std::string("HEAD");
		break;
	case kPut:
		return std::string("PUT");
		break;
	case kDelete:
		return std::string("DELETE");
		break;
	default:
		return std::string("UNKNOWN");
		break;		
	}
}

void HttpRequest::addHeader(const std::string& field, const std::string& value)
{
	std::string headerField = StringUtils::trimSpace(field);
	std::string headerValue = StringUtils::trimSpace(value);
	mHttpHeaders[headerField] = headerValue;
}

std::string HttpRequest::getHeader(const std::string& field) const
{
	std::string result;
	std::map<string, string>::const_iterator it = mHttpHeaders.find(field);
	if (it != mHttpHeaders.end())
	{
	  result = it->second;
	}
	return result;
}

void HttpRequest::swap(HttpRequest& that)
{
	std::swap(mMethod, that.mMethod);
	mRequestPath.swap(that.mRequestPath);
	mQuery.swap(that.mQuery);
	mHttpHeaders.swap(that.mHttpHeaders);
}

