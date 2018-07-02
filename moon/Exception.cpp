#include <moon/Exception.h>

#include <cxxabi.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

using namespace moon;

Exception::Exception(const char* what) : mMessage(what)
{
	fillStackTrace();
}

Exception::Exception(const std::string& what) : mMessage(what)
{
	fillStackTrace();
}

Exception::~Exception() throw()
{
}

const char* Exception::what() const throw()
{
	return mMessage.c_str();
}

const char* Exception::stackTrace() const throw()
{
	return mStack.c_str();
}

void Exception::fillStackTrace()
{
	const int len = 200;
	void* buffer[len];
	int nptrs = ::backtrace(buffer, len);
	char ** strings = ::backtrace_symbols(buffer, nptrs);
	if (NULL != strings)
	{
		for (int i = 0; i < nptrs; ++i)
		{
			mStack.append(demangle(strings[i]));
			mStack.push_back('\n');
		}
		free(strings);
	}
}

//反名字改编
std::string Exception::demangle(const char* symbol)
{
    size_t size;
    int status;
    char temp[128];
    char* demangled;
    //first, try to demangle a c++ name
    if (1 == sscanf(symbol, "%*[^(]%*[^_]%127[^)+]", temp)) {
        if (NULL != (demangled = abi::__cxa_demangle(temp, NULL, &size, &status))) {
          std::string result(demangled);
          free(demangled);
          return result;
        }
    }
    //if that didn't work, try to get a regular c symbol
    if (1 == sscanf(symbol, "%127s", temp)) {
        return temp;
    }

    //if all else fails, just return the symbol
    return symbol;
}