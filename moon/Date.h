/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_DATE_H_
#define MOON_DATE_H_

#include <time.h>
#include <limits>
#include <string>

namespace moon {

/**
  一个Date对象表示一个日期，包含了年、月、日信息，日期表示以公历为标准。
  可以从系统时钟获取当前的日期。Date对象支持日期的比较，以及对日期的操作，例如：
  判断两个日期是否相等，可以增加日期的天数、月数以及年数。
*/
class Date
{
private:
	explicit Date(long jd) : mJulianDayNumber(jd) {}
public:
	Date() : mJulianDayNumber(nullJd()) {}	
    Date(int year, int month, int day);
    
	/**
	  返回true表示日期为null，否则返回false。一个null的日期是无效的。
	*/
	bool isNull() const { return !isValid(); }

	/**
	  返回true表示当前日期有效，返回false表示日期无效。
	*/
    bool isValid() const { return mJulianDayNumber >= minJd() && mJulianDayNumber <= maxJd(); }
    
	/**
	  返回一个整数表示当前日期的年份。正数表示公元后，负数表示公元前。
	  返回0表示日期无效。
	*/
	int year() const;

	/**
	  返回一个正整数表示当前日期的月份，范围(1到12)。例如：返回4表示4月份
	  返回0表示日期无效。
	*/
    int month() const;

	/**
	  返回一个正整数表示当前日期的日号，范围(1到31)。
	  返回0表示日期无效
	*/
    int day() const;

	/**
	  返回一个正整数表示当前日期的星期数（范围1到7，1=星期一，7表示星期日）
	  返回0表示日期无效。
	*/
    int dayOfWeek() const;

	/**
	  返回一个正整数表示日期所在年已过去的天数，包含当天，范围(1到365，如果是闰年可能返回366)
	  例如:日期2018-01-01，返回1，表示已经过去了1天，日期为2018-03-11，返回70（31+28+11）。
	  返回0表示日期无效。
	*/
    int dayOfYear() const;

	/**
	  返回一个正整数表示当前月份的总天数，范围（28到31）。例如：2018-02-01该日期返回2月份的总天数，返回28
	  返回0表示日期无效。
	*/
    int daysInMonth() const;

	/**
	  返回一个正整数表示当前年份的总天数，平常年返回365，闰年时返回366。
	  返回0表示日期无效。
	*/
    int daysInYear() const;
    
	/**
	  如果days为正数，返回一个比当前日期晚@days天的@Date对象，
	  如果days为负数，返回一个比当前日期早@days天的@Date对象。
	  如果当前日期无效，或者新的日期超出了范围，则返回一个null的@Date对象。
	*/
    Date addDays(int days) const;

	/**
	  如果months为正数，返回一个比当前日期晚@months月的@Date对象，
	  如果months为负数，返回一个比当前日期早@months月的@Date对象。
	  如果当前日期无效，或者新的日期超出了范围，则返回一个null的@Date对象。

	  Note: 如果当前的月/日不符合新结果的月/日，方法将会将月/日调整到当月最后的有效月/日。
	  例如:2018-01-31向后添加一个月，此时不会变成2018-02-31，而是2018-02-28。
	    如果是2018-02-28然后减去一个月(参数months为-1)，此时变成2018-01-28而不是2018-01-31。
	*/
	Date addMonths(int months) const;

	/**
	  如果years为正数，返回一个比当前日期晚@years年的@Date对象，
	  如果years为负数，返回一个比当前日期早@years年的@Date对象。
	  如果当前日期无效，或者新的日期超出了范围，则返回一个null的@Date对象。

	  Note: 如果当前的月/日不符合新结果的月/日，方法将会将月/日调整到当月最后的有效月/日。
	  例如:2016-02-29向后添加一年，此时不会变成2017-02-29，而是2017-02-28。
	*/
	Date addYears(int years) const;
    
	/**
	  将日期设置为指定的日期。返回true表示设置成功，
	  返回false表示设置失败，表示设置的日期(year, month, day)无效，此时日期对象将变得无效@isValid()方法将返回true。
	*/
	bool setDate(int year, int month, int day);
    
	// 返回一个long类型的整数，表示当前日期的儒略日。
	long julianDayNumber() const {return mJulianDayNumber;}
   
	// 返回一个描述当前日期的std::string对象，格式为yyyy-mm-dd
    std::string toIsoString() const;

    // 返回一个描述当前日期的std::string对象，格式为yyyymmdd
	std::string toString() const;
	
	// 将儒略日@jd转换为一个@Date对象。
	static Date fromJulianDay(long jd){
        return (jd >= minJd() && jd <= maxJd()) ? Date(jd) : Date();
	}
    
	// 返回一个@Date对象表示当前日期
    static Date currentDate();

	/**
	  返回true表示日期(year, month, day)有效，返回false表示无效。
	  @params year 表示年份，可以为任意正数或者负数，但是不能为0。
	  @parmas month 表示月份，必须在[1, 12]范围内。
	  @params day 表示当前月份的总天数，1,3,5,7,8,10,12月份天数为31天，
	    其它除2月份的月份天数为30天，2月份天数为28天，如果year表示的年份为闰年，2月份天数为29.
	  
	  @return 返回true表示当前year,month,day组合合法。
	*/
	static bool isValid(int year, int month, int day);

	/**
	  返回true表示@year为闰年，返回false为平常年。
      参数year可为负数，表示公元前。例如-1表示公元前1年，
	  -5表示公元前4年，-5表示的是闰年，同理-9也是闰年。
	*/
	static bool isLeapYear(int year);

private:
	static inline long nullJd() { return (std::numeric_limits<long>::min)(); }
	static inline long minJd() { return -784350574879L; }
    static inline long maxJd() { return 784354017364L; }
private:	
	long mJulianDayNumber;
};

inline bool operator==(Date x, Date y){
  return x.julianDayNumber() == y.julianDayNumber();
}

inline bool operator!=(Date x, Date y){
  return x.julianDayNumber() != y.julianDayNumber();
}

inline bool operator<(Date x, Date y){
  return x.julianDayNumber() < y.julianDayNumber();
}

inline bool operator<=(Date x, Date y){
  return x.julianDayNumber() <= y.julianDayNumber();
}

inline bool operator>(Date x, Date y){
  return x.julianDayNumber() > y.julianDayNumber();
}

inline bool operator>=(Date x, Date y){
  return x.julianDayNumber() >= y.julianDayNumber();
}


}  // ~moon
#endif // ~