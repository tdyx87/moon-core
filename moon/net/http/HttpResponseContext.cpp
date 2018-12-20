#include <moon/net/http/HttpResponseContext.h>
#include <moon/net/Buffer.h>
#include <moon/Logger.h>
#include <moon/Number.h>
#include <moon/StringUtils.h>

using namespace moon;
using namespace moon::net;

bool HttpResponseContext::parseResponse()
{
	bool ok = true;
	//bool hasMore = true;

	if (kExpectStatusLine == mState) {
		ok = parseStatusLine();
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

bool HttpResponseContext::parseStatusLine()
{
	const char *end = mBuffer->findCRLF();
	if (NULL == end)
	{
        return true;  // 表示解析成功，因为数据HTTP响应数据可能尚未接受完全
	}

    const char *start = mBuffer->data();
	const char *space = std::find(start, end, ' ');
	if (space == end)
	{
		return false;  // HTTP状态行无效
	}
	string httpVersion(start, space);  // 获取响应行HTTP版本信息

	start = space + 1;
	space = std::find(start, end, ' ');
	if (space == end)
	{
		return false;  // HTTP状态行无效
	}

	string statusCode(start, space);
	if (statusCode.length() != 3)
	{
        return false;  // 响应码不正确
	}

	int iStatusCode = Number::parseInt(statusCode);
	mResponse.setStatusCode(iStatusCode);

    mBuffer->retrieveUntil(end + 2);  // 将请求头信息移出数据缓冲区
	mState = kExpectHeaders;          // 设置下个解析状态为解析请求头

	return true;
}

// 解析消息报头
bool HttpResponseContext::parseHeader()
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
			mResponse.addHeader(field, value);
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
bool HttpResponseContext::parseBody()
{
	string contentLengthStr = mResponse.getHeader("Content-Length");
	if (contentLengthStr.empty())
	{
		mState = kGotAll;
		return true;
	}

	size_t length = Number::parseInt(contentLengthStr);
	if (length == 0) {
		LOGGER_WARN("Content-Length[%s], invalid value!", contentLengthStr.c_str());
		return false;
	}

	if (length > mBuffer->readableBytes())
	{
		LOGGER_DEBUG("Not receive completed, content length:%s, but readableBytes:%ld", contentLengthStr.c_str(), mBuffer->readableBytes());
		return true;  // 数据尚未接受完毕
	}

	mResponse.setBody(std::string(mBuffer->data(), mBuffer->data() + length));
	mState = kGotAll;
	return true;
}



