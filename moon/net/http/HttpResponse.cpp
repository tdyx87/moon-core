#include <moon/net/http/HttpResponse.h>
#include <moon/net/http/HttpStatusCode.h>

#include <stdio.h>

using namespace moon;
using namespace moon::net;

using std::string;

void HttpResponse::swap(HttpResponse& that)
{
	mHttpHeaders.swap(that.mHttpHeaders);
	mHttpBody.swap(that.mHttpBody);
}

std::string HttpResponse::getHeader(const std::string& field) const
{
	std::string result;
	std::map<string, string>::const_iterator it = mHttpHeaders.find(field);
	if (it != mHttpHeaders.end()) {
	  result = it->second;
	}
	return result;
}

std::string HttpResponse::toString()
{	
    char buf[32] = {0};
    snprintf(buf, sizeof buf, "HTTP/1.1 %d ", mStatusCode);

    string statusMessage = HttpStatusCode::getCodeInfo(mStatusCode);
	std::string response = "";
    response.append(buf);
    response.append(statusMessage);
    response.append("\r\n");
    
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", mHttpBody.size());
    response.append(buf);
 
    for (std::map<string, string>::const_iterator it = mHttpHeaders.begin(); it != mHttpHeaders.end(); ++it) {
        response.append(it->first);
        response.append(": ");
        response.append(it->second);
        response.append("\r\n");
    }

    response.append("\r\n");
    response.append(mHttpBody);

    return response;
}

std::string HttpResponse::getDefaultResponse(const std::string& body)
{
	HttpResponse response;
	response.setStatusCode(HttpStatusCode::k200Ok);
	response.addHeader("Content-Type", "text/html");
	response.setBody(body);

	return response.toString();
}
	
std::string HttpResponse::getErrorDefaultResponse(int code)
{
	HttpResponse response;
	response.setStatusCode(code);

	return response.toString();
}
