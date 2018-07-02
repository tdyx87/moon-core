#include <moon/Integer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits>

using namespace moon;

const int Integer::MAX_VALUE = std::numeric_limits<int>::max();  // int能表示的最大值
const int Integer::MIN_VALUE = std::numeric_limits<int>::min();  // int能表示的最小值

//static const int kDefaultRadix = 10;

int Integer::parseInt(const std::string& str, int def, int radix )
{
	if (str.empty())
	{
		return def;
	}
	return parseInt(str.c_str(), def, radix);
}

int Integer::parseInt(const char* str, int def, int radix)
{
	if ( (str == NULL) || (strlen(str) == 0) )
	{
		return def;
	}
    
	char *endptr = NULL;
	const long ret = strtol(str, &endptr, radix);
	if (endptr == str)
	{
		return def;
	}
	if (ret > MAX_VALUE){
		return MAX_VALUE;
	}
	else if (ret < MIN_VALUE){
		return MIN_VALUE;
	}

	return static_cast<int>(ret);
}

std::string Integer::toString(int i)
{
	char szTmp[16] = {0};
	sprintf(szTmp, "%d", i);
	return std::string(szTmp);
}
