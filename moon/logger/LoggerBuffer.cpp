#include <moon/logger/LoggerBuffer.h>

#include <string.h> 

namespace moon 
{

// Buffer最大不能超过64k，超过以64k为准
static const size_t kMaxBufferSize = 65536; // 64 * 1024 = 64k

LoggerBuffer::LoggerBuffer(size_t size) : mMaxSize(size < kMaxBufferSize ? size : kMaxBufferSize)
{
    mData = new char[mMaxSize];
	mSize = 0;
	memset(mData, '\0', mMaxSize);
}

LoggerBuffer::~LoggerBuffer()
{
    delete [] mData;
	mData = NULL;
}

LoggerBuffer::LoggerBuffer(const LoggerBuffer &rhs) : mMaxSize(rhs.mMaxSize)
{
    mSize = rhs.mSize;
	mData = new char[mMaxSize];
	memcpy(mData, rhs.mData, mSize);
}

LoggerBuffer& LoggerBuffer::operator=(const LoggerBuffer &rhs)
{
    mSize = rhs.mSize;
	mData = new char[mMaxSize];
	memcpy(mData, rhs.mData, mSize);

	return *this;
}
	
void LoggerBuffer::append(const char *buf)
{
    if (NULL == buf)
    {
        return ;
    }
	append(buf, strlen(buf));
}

void LoggerBuffer::append(const char *buf, size_t len)
{
    if (NULL == buf)
    {
        return ;
    }
	
    if (available() >= len)
    {
		memcpy(current(), buf, len);
		mSize += len;
    }
	else
	{
		memcpy(current(), buf, available());
		mSize += available();
	}
}


}  // ~moon