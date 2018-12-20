#include <moon/net/http/HttpContext.h>
#include <moon/net/Buffer.h>
#include <moon/Logger.h>
#include <moon/Number.h>
#include <moon/StringUtils.h>

using namespace moon;
using namespace moon::net;

// 解析HTTP请求
bool HttpContext::parseRequest()
{
	bool ok = true;
	//bool hasMore = true;

	// 解析请求行
	if (kExpectRequestLine == mState) {
		ok = parseRequestLine();
		if (!ok) {
			return false;
		}
	}

	// 解析请求报头
	if (kExpectHeaders == mState) {
		ok = parseHeader();
		if (!ok) {
			return false;
		}
	}	

	// 解析请求正文
	if (kExpectBody == mState) {
		ok = parseBody();
		if (!ok) {
			return false;
		}
	}

	return true;
}

// 解析请求行
bool HttpContext::parseRequestLine()
{		
	const char *end = mBuffer->findCRLF();
	if (NULL == end) {
        return true;  // 表示解析成功，因为数据HTTP请求头数据可能尚未接受完全
	}

    const char *start = mBuffer->data();
	const char *space = std::find(start, end, ' ');
	if (space == end) {
		return false;  // HTTP请求行无效
	}
	// 设置HTTP请求方法
	if ( !mRequest.setMethod(std::string(start, space)) ) {
		return false;  // HTTP请求方法无效
	}

	start = space + 1;
	space = std::find(start, end, ' ');
	if (space == end) {
		return false;  // HTTP请求行无效
	}
	
	// 设置请求路径和请求内容
	const char *question = std::find(start, space, '?');
	if (question != space) {
		mRequest.setPath(std::string(start, question));
		mRequest.setQuery(std::string(question + 1, space));
	} 
	else {
		mRequest.setPath(std::string(start, space));
	}

    start = space + 1;
	const bool succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
	if (!succeed) {
		return false;  // HTTP请求行无效
	}

	if (*(end-1) == '1') {
		mRequest.setVersion(HttpRequest::kHttp11);
	}
	else if (*(end-1) == '0') {
		mRequest.setVersion(HttpRequest::kHttp10);
	}
	else {
	    return false;    // HTTP请求行无效
	}

    mBuffer->retrieveUntil(end + 2);  // 将请求头信息移出数据缓冲区
	mState = kExpectHeaders;          // 设置下个解析状态为解析请求头

	return true;
}

// 解析消息报头
bool HttpContext::parseHeader()
{
	bool hasMore = true;
	while (hasMore) {
		const char *crlf = mBuffer->findCRLF();
		if (NULL == crlf) {
			return true;  // 表示解析成功，因为数据HTTP请求头数据可能尚未接受完全
		}
		const char *colon = std::find(mBuffer->data(), crlf, ':');
		if (colon != crlf) {
			std::string field(StringUtils::trimSpace(std::string(mBuffer->data(), colon)) );
			std::string value(StringUtils::trimSpace(std::string(colon + 1, crlf)) );
			mRequest.addHeader(field, value);
		}
		else {
			mState = kExpectBody;  // 设置下个解析状态为解析请求正文
			hasMore = false;       // 请求头解析完毕
		}

		mBuffer->retrieveUntil(crlf + 2);
	}
	
	return true;
}

// 解析请求正文
bool HttpContext::parseBody()
{
	string contentLengthStr = mRequest.getHeader("Content-Length");
	if (contentLengthStr.empty()) {
		mState = kGotAll;
		return true;
	}

	size_t length = Number::parseInt(contentLengthStr);
	if (length == 0) {
		LOGGER_WARN("Content-Length[%s], invalid value!", contentLengthStr.c_str());
		return false;
	}

	if (length > mBuffer->readableBytes()) {
		LOGGER_DEBUG("Not receive completed, content length:%s, but readableBytes:%ld", contentLengthStr.c_str(), mBuffer->readableBytes());
		return true;  // 数据尚未接受完毕
	}

	mRequest.setBody(std::string(mBuffer->data(), mBuffer->data() + length));
	mState = kGotAll;
	return true;
}



