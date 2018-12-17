#include <moon/Date.h>

#include <stdio.h>
#include <time.h>

namespace moon  {

static const int MONTH_PER_YEAR = 12;
static const char MONTH_DAYS[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

//static inline long floordiv(long a, int b) {
//    return (a - (a < 0 ? b - 1 : 0)) / b;
//}

static inline int floordiv(long a, int b) {
    return static_cast<int>((a - (a < 0 ? b - 1 : 0)) / b);
}

static inline int floordiv(int a, int b) {
    return (a - (a < 0 ? b - 1 : 0)) / b;
}

static inline long getJulianDayNumber(int year, int month, int day) {
   // Adjust for no year 0
    if (year < 0) {
        ++year;
	}

	int a = floordiv(14 - month, 12);
    long y = static_cast<long>(year) + 4800 - a;
    int m = month + 12 * a - 3;
    return day + floordiv(153 * m + 2, 5) + 365 * y + floordiv(y, 4) - floordiv(y, 100) + floordiv(y, 400) - 32045;
}

static inline int min(int lhs, int rhs) {
	return (lhs > rhs) ? rhs : lhs;
}

static inline Date fixedDate(int year, int month, int day)
{
    Date result(year, month, 1);
    result.setDate(year, month, min(day, result.daysInMonth()));
    return result;
}

struct YearMonthDay
{
    int year, month, day;
};

static YearMonthDay getDateFromJulianDay(long julianDay)
{	
	long a = julianDay + 32044;
    long b = floordiv(4 * a + 3, 146097);
    int  c = static_cast<int>(a - floordiv(146097 * b, 4));

    int    d = floordiv(4 * c + 3, 1461);
    int    e = c - floordiv(1461 * d, 4);
    int    m = floordiv(5 * e + 2, 153);

    int    day = e - floordiv(153 * m + 2, 5) + 1;
    int    month = m + 3 - 12 * floordiv(m, 10);
    int    year = static_cast<int>(100 * b + d - 4800 + floordiv(m, 10));

    // Adjust for no year 0
    if (year <= 0){
        --year ;
	}

    YearMonthDay result = { year, month, day };
    return result;
}

Date::Date(int year, int month, int day)
{
	setDate(year, month, day);
}

int Date::year() const
{
	if (isNull()){		
        return 0;
	}

    return getDateFromJulianDay(mJulianDayNumber).year;
}

int Date::month() const
{
	if (isNull()){		
        return 0;
	}

    return getDateFromJulianDay(mJulianDayNumber).month;
}

int Date::day() const
{
	if (isNull()){		
        return 0;
	}

    return getDateFromJulianDay(mJulianDayNumber).day;
}

int Date::dayOfWeek() const
{
    if (isNull()){
        return 0;
	}

    if (mJulianDayNumber >= 0){
        return static_cast<int>((mJulianDayNumber % 7) + 1);
	}
    return static_cast<int>(((mJulianDayNumber + 1) % 7) + 7);
}

int Date::dayOfYear() const
{
	if (isNull()){
		return 0;
	}

	return static_cast<int>(mJulianDayNumber - getJulianDayNumber(year(), 1, 1) + 1);
}

int Date::daysInMonth() const
{
	if (isNull()){
        return 0;
	}

    const YearMonthDay ymd = getDateFromJulianDay(mJulianDayNumber);
    if (ymd.month == 2 && isLeapYear(ymd.year)){
        return 29;
	}
    return MONTH_DAYS[ymd.month];
}

int Date::daysInYear() const
{
	if (isNull()) {
        return 0;
	}

    return isLeapYear(getDateFromJulianDay(mJulianDayNumber).year) ? 366 : 365;
}

Date Date::addDays(int days) const
{
	if (isNull()){
		return Date();
	}
	return fromJulianDay(mJulianDayNumber + days);
}

Date Date::addMonths(int months) const
{	
	if (!isValid()) {
        return Date();
	}
    if (0 == months) {
        return *this;
	}

	int year, month, day;
	{
        const YearMonthDay ymd = getDateFromJulianDay(mJulianDayNumber);
		year = ymd.year;
		month = ymd.month;
		day = ymd.day;
	}
	const int oldYear = year;
	const bool increasing = months > 0;

	while (months != 0)	{
		if ( months < 0 && months+12 <= 0) {
			--year;
			months += 12;
		} else if (months < 0) {
			month += months;
            months = 0;
			if (month <= 0)	{
				--year;
				month += 12;
			}
		} else if ( months - 12 >= 0 ) {
			++year;
			months -= 12;
		} else {
			// condition for (0 < months && months - 12 < 0 )
			month += months;
			months = 0;
			if (month > 12) {
				++year;
				month -= 12;
			}
		}  // ~if ( months < 0 && months+12 <= 0)
	}  // ~while (months != 0)

	const bool hasPassedCE = (oldYear > 0 && year <= 0) || (oldYear < 0 && year >= 0);
	if (hasPassedCE){
        year += increasing ? +1 : -1;
	}

	return fixedDate(year, month, day); 
}

Date Date::addYears(int years) const
{
    if (isNull()) {
		return Date();
    }

	const YearMonthDay ymd = getDateFromJulianDay(mJulianDayNumber);
	int targetYear = ymd.year + years;
    
	// 当年份从公元前跨到公元后，年份需要+1，如果从公元后进入到了公元前，年份需要-1。
	const bool hasPassedCE = (ymd.year > 0 && targetYear <= 0) || (ymd.year < 0 && targetYear >= 0);
	if (hasPassedCE) {
        targetYear += (years > 0) ? +1 : -1;
	}
	//printf("targetYear[%d] ymd.month[%d] ymd.day[%d].\n", targetYear, ymd.month, ymd.day);

    return fixedDate(targetYear, ymd.month, ymd.day); 
}

bool Date::setDate(int year, int month, int day)
{
	if (isValid(year, month, day)) {
		mJulianDayNumber = getJulianDayNumber(year, month, day);
	}else{
		mJulianDayNumber = nullJd();
	}

	return isValid();
}

std::string Date::toIsoString() const
{
    char buf[32];
    const YearMonthDay ymd = getDateFromJulianDay(mJulianDayNumber);
    snprintf(buf, sizeof buf, "%4d-%02d-%02d", ymd.year, ymd.month, ymd.day);
    return buf;
}

std::string Date::toString() const
{
	char buf[32];
    const YearMonthDay ymd = getDateFromJulianDay(mJulianDayNumber);
    snprintf(buf, sizeof buf, "%4d%02d%02d", ymd.year, ymd.month, ymd.day);
    return buf;
}

Date Date::currentDate()
{
	time_t curTime = time(NULL); 
	struct tm t;
    localtime_r(&curTime, &t);
    
	Date d;
	d.mJulianDayNumber = getJulianDayNumber(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
	return d;
}

bool Date::isValid(int year, int month, int day)
{
	if (0 == year) {
		return false;
	}
	return (day > 0) && (0 < month && month <= 12) &&
		( day <= MONTH_DAYS[month] || (day == 29 && isLeapYear(year)) );
}

bool Date::isLeapYear(int year)
{
	// No year 0 in Gregorian calendar, so -1, -5, -9 etc are leap years
	if ( year < 1)	{
		++year;
	}

	return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}


}  // ~moon