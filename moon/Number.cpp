#include <moon/Number.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits>
#include <sstream>

namespace moon
{
const int    Number::kIntMax    = std::numeric_limits<int>::max();  
const int    Number::kIntMin    = std::numeric_limits<int>::min();  
const long   Number::kLongMax   = std::numeric_limits<long>::max();  
const long   Number::kLongMin   = std::numeric_limits<long>::min(); 
const double Number::kDoubleMax = std::numeric_limits<double>::max();  
const double Number::kDoubleMin = std::numeric_limits<double>::min(); 

const double Number::kEpsinon = 0.000001;

int Number::parseInt(const std::string& str, int def, int radix )
{
	if (str.empty())
	{
		return def;
	}
	return parseInt(str.c_str(), def, radix);
}

int Number::parseInt(const char* str, int def, int radix)
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
	if (ret > Number::kIntMax){
		return Number::kIntMax;
	}
	else if (ret < Number::kIntMin){
		return Number::kIntMin;
	}
	return static_cast<int>(ret);
}

long Number::parseLong(const std::string& str, long def, int radix)
{
	if (str.empty())
	{
		return def;
	}
	return parseLong(str.c_str(), def, radix);
}

long Number::parseLong(const char* str, long def, int radix)
{
	if (str == NULL)
	{
		return def;
	}

	return strtol(str, NULL, radix);
}

double Number::parseDouble(const std::string& str, double def)
{
	if (str.empty())
	{
		return def;
	}
	return parseDouble(str.c_str());
}

double Number::parseDouble(const char* str, double def)
{
	if (str == NULL)
	{
		return def;
	}

	char *endptr = NULL;
	const double ret = strtod(str, &endptr);
	if (endptr == str)
	{
		return def;
	}

	return ret;
}

int Number::compare(double d1, double d2)
{
	double dif = d1 - d2;
	if ( dif > kEpsinon)
	{
		return 1;    // d1 greater than d2
	}
	else if (dif < -kEpsinon)
	{
		return -1;   // d1 less than d2
	}
	else
	{
		return 0;    // d1 equal to d2
	}
}

std::string Number::intToString(int i)
{
	char szTmp[16] = {0};
	sprintf(szTmp, "%d", i);
	return std::string(szTmp);
}

std::string Number::longToString(long l)
{
	char szTmp[64] = {0};
	sprintf(szTmp, "%ld", l);
	return std::string(szTmp);
}

std::string Number::doubleToString(double d)
{
	std::ostringstream os;
	string str("0.0");
	if (os << d){
		str = os.str();
	}

	return str; 
}

}//~ moon