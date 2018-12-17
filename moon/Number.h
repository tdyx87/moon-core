#ifndef MOON_NUMBER_H_
#define MOON_NUMBER_H_

#include <string>

namespace moon
{
using std::string;

class Number
{
public:
    static int parseInt(const std::string& str, int def = 0, int radix = 10);
	static int parseInt(const char* str, int def = 0, int radix = 10);

	static long parseLong(const std::string& str, long def = 0L, int radix = 10);
	static long parseLong(const char* str, long def = 0L, int radix = 10);

	// 将字符串str转换为一个double数据，如果字符串为空返回def指定的值
	static double parseDouble(const std::string& str, double def = kEpsinon);
    
	// 将字符串str转换为一个double数据，如果(str==null || strlen(str)==0)为空返回def指定的值
	static double parseDouble(const char* str, double def = kEpsinon);

	// 比较d1和d2, 返回1表示d1>d2, 返回0表示d1==d2,返回-1表示d1<d2
	static int compare(double d1, double d2);

	static std::string intToString(int i);
	static std::string longToString(long l);
	static std::string doubleToString(double d);

public:
	static const int kIntMax;  
	static const int kIntMin;  
	static const long kLongMax;
	static const long kLongMin;
	static const double kDoubleMax;
	static const double kDoubleMin;

	static const double kEpsinon;  
};  //~ Number

}  //~ moon

#endif