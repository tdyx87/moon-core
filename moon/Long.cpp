#include <moon/Long.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits>

namespace moon
{

const long Long::MAX_VALUE = std::numeric_limits<long>::max();  
const long Long::MIN_VALUE = std::numeric_limits<long>::min(); 

long Long::parseLong(const std::string& str, long def, int radix)
{
	if (str.empty())
	{
		return def;
	}
	return parseLong(str.c_str(), def, radix);
}

long Long::parseLong(const char* str, long def, int radix)
{
	if (str == NULL)
	{
		return def;
	}

	return strtol(str, NULL, radix);
}

std::string Long::toString(long l)
{
	char szTmp[64] = {0};
	sprintf(szTmp, "%ld", l);
	return std::string(szTmp);
}


}  //~ moon