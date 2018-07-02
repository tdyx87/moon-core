#include <moon/logger/LoggerStream.h>

#include <boost/static_assert.hpp>

#include <algorithm>
#include <limits>

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

namespace moon 
{

static const int kMaxNumericSize = 32;

static const char digits[] = "9876543210123456789";
static const char* zero = digits + 9;
BOOST_STATIC_ASSERT(sizeof(digits) == 20);

static const char digitsHex[] = "0123456789ABCDEF";
BOOST_STATIC_ASSERT(sizeof digitsHex == 17);

template<typename T> size_t convert(char buf[], T value)
{
    T i = value;
    char* p = buf;

    do
    {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (value < 0)
    {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

size_t convertHex(char buf[], uintptr_t value)
{
    uintptr_t i = value;
    char* p = buf;

    do
    {
        int lsd = i % 16;
        i /= 16;
        *p++ = digitsHex[lsd];
    } while (i != 0);

    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

template<typename T> void LoggerStream::formatInteger(T v)
{
  if (mBuffer->available() >= kMaxNumericSize)
  {
      char buffer[32] = {0};
      size_t len = convert(mBuffer->current(), v);
      mBuffer->add(len);
  }
}

LoggerStream& LoggerStream::logging(const char* format, ...)
{
	va_list ap;
    va_start(ap, format);
	int n = vsnprintf(mBuffer->current(), mBuffer->available(), format, ap);
    va_end(ap);	
    
	mBuffer->add(n > 0 ? n : 0);

	return *this;
}

LoggerStream& LoggerStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}

LoggerStream& LoggerStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}

LoggerStream& LoggerStream::operator<<(int v)
{
    formatInteger(v);
    return *this;
}

LoggerStream& LoggerStream::operator<<(unsigned int v)
{
    formatInteger(v);
    return *this;
}

LoggerStream& LoggerStream::operator<<(long v)
{
    formatInteger(v);
    return *this;
}

LoggerStream& LoggerStream::operator<<(unsigned long v)
{
    formatInteger(v);
    return *this;
}

LoggerStream& LoggerStream::operator<<(long long v)
{
    formatInteger(v);
    return *this;
}

LoggerStream& LoggerStream::operator<<(unsigned long long v)
{
    formatInteger(v);
    return *this;
}

LoggerStream& LoggerStream::operator<<(const void* p)
{
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    if (mBuffer->available() >= kMaxNumericSize)
    {
        char* buf = mBuffer->current();
        buf[0] = '0';
        buf[1] = 'x';
        size_t len = convertHex(buf+2, v);
        mBuffer->add(len+2);
    }
    return *this;
}

LoggerStream& LoggerStream::operator<<(double v)
{
    if (mBuffer->available() >= kMaxNumericSize)
    {
        int len = snprintf(mBuffer->current(), kMaxNumericSize, "%.12g", v);
        mBuffer->add(len);
    }
    return *this;
}

} // ~moon

