#include <moon/Double.h>

#include <stdio.h>
#include <stdlib.h>

#include <limits>
#include <sstream>

using namespace moon;

const double Double::dEPSINON;  
const double Double::MAX_VALUE = std::numeric_limits<double>::max();  // double能表示的最大值
const double Double::MIN_VALUE = std::numeric_limits<double>::min();  // double能表示的最小值

double Double::parseDouble(const std::string& str, double def)
{
	if (str.empty())
	{
		return def;
	}
	return parseDouble(str.c_str());
}

double Double::parseDouble(const char* str, double def)
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

std::string Double::toString(double d)
{
	std::ostringstream os;
	string str("0.0");
	if (os << d){
		str = os.str();
	}

	return str;
	//char szTmp[64] = {0};
	//sprintf(szTmp, "%f", d);
	//printf("d[%f] str[%s]\n", d, szTmp);
	//return std::string(szTmp);
	 
}

int Double::compare(double d1, double d2)
{
	double dif = d1 - d2;
	if ( dif > dEPSINON)
	{
		return 1;    // d1 greater than d2
	}
	else if (dif < -dEPSINON)
	{
		return -1;   // d1 less than d2
	}
	else
	{
		return 0;    // d1 equal to d2
	}
}

