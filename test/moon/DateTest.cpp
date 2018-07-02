#include <moon/Date.h>

#include <gtest/gtest.h>

using namespace moon;

TEST(DateTest, isNull){
	Date date(0, 1, 1);
	EXPECT_TRUE(date.isNull());
}

TEST(DateTest, isValid){
	Date date1(2018, 5, 3);
	EXPECT_TRUE(date1.isValid());
}

TEST(DateTest, yeadMonthDay){
	Date date(2018, 5, 3);
	
	EXPECT_EQ(2018, date.year());
	EXPECT_EQ(5, date.month());
	EXPECT_EQ(3, date.day());
}

TEST(DateTest, dayOfWeek){
	Date date(2018, 5, 3);
	
	EXPECT_EQ(4, date.dayOfWeek());
}

TEST(DateTest, dayOfYear){
	Date date(2018, 5, 3);
	
	EXPECT_EQ(123, date.dayOfYear());  // 31 + 28 + 31 + 30 + 3
}

TEST(DateTest, daysInMonth){
	Date date(2018, 5, 3);
	
	EXPECT_EQ(31, date.daysInMonth());  
}

TEST(DateTest, daysInYear){
	Date date(2018, 5, 3);
	
	EXPECT_EQ(365, date.daysInYear());  
}

TEST(DateTest, addDays){
	Date date(2018, 5, 3);

	Date newdate1 = date.addDays(3);	
	EXPECT_EQ(2018, newdate1.year());
	EXPECT_EQ(5, newdate1.month());
	EXPECT_EQ(6, newdate1.day());  

	Date newdate2 = date.addDays(30);	
	EXPECT_EQ(2018, newdate2.year());
	EXPECT_EQ(6, newdate2.month());
	EXPECT_EQ(2, newdate2.day());  

	Date newdate3 = date.addDays(365);	
	EXPECT_EQ(2019, newdate3.year());
	EXPECT_EQ(5, newdate3.month());
	EXPECT_EQ(3, newdate3.day()); 
}

TEST(DateTest, subDays){
	Date date(2018, 5, 3);

	Date newdate1 = date.addDays(-2);	
	EXPECT_EQ(2018, newdate1.year());
	EXPECT_EQ(5, newdate1.month());
	EXPECT_EQ(1, newdate1.day());  

	Date newdate2 = date.addDays(-30);	
	EXPECT_EQ(2018, newdate2.year());
	EXPECT_EQ(4, newdate2.month());
	EXPECT_EQ(3, newdate2.day());  

	Date newdate3 = date.addDays(-365);	
	EXPECT_EQ(2017, newdate3.year());
	EXPECT_EQ(5, newdate3.month());
	EXPECT_EQ(3, newdate3.day()); 
}

TEST(DateTest, addMonths){
	Date date(2018, 1, 31);

	Date newdate1 = date.addMonths(1);	
	EXPECT_EQ(2018, newdate1.year());
	EXPECT_EQ(2, newdate1.month());
	EXPECT_EQ(28, newdate1.day());  

	Date newdate2 = date.addMonths(12);	
	EXPECT_EQ(2019, newdate2.year());
	EXPECT_EQ(1, newdate2.month());
	EXPECT_EQ(31, newdate2.day());  
}

TEST(DateTest, subMonths){
	Date date(2018, 2, 28);

	Date newdate1 = date.addMonths(-1);	
	EXPECT_EQ(2018, newdate1.year());
	EXPECT_EQ(1, newdate1.month());
	EXPECT_EQ(28, newdate1.day());  

	Date newdate2 = date.addMonths(-12);	
	EXPECT_EQ(2017, newdate2.year());
	EXPECT_EQ(2, newdate2.month());
	EXPECT_EQ(28, newdate2.day());  
}

TEST(DateTest, addYears){
	Date date(-1, 1, 31);

	Date newdate1 = date.addYears(1);	
	EXPECT_EQ(1, newdate1.year());
	EXPECT_EQ(1, newdate1.month());
	EXPECT_EQ(31, newdate1.day());  

	Date newdate2 = date.addYears(2018);	
	EXPECT_EQ(2018, newdate2.year());
	EXPECT_EQ(1, newdate2.month());
	EXPECT_EQ(31, newdate2.day());  
}

TEST(DateTest, subYears){
	Date date(2018, 1, 31);

	Date newdate1 = date.addYears(-1);	
	EXPECT_EQ(2017, newdate1.year());
	EXPECT_EQ(1, newdate1.month());
	EXPECT_EQ(31, newdate1.day());  

	Date newdate2 = date.addYears(-2018);	
	EXPECT_EQ(-1, newdate2.year());
	EXPECT_EQ(1, newdate2.month());
	EXPECT_EQ(31, newdate2.day());  
}

TEST(DateTest, setDate){
	Date date(2018, 1, 31);
	EXPECT_EQ(2018, date.year());
	EXPECT_EQ(1, date.month());
	EXPECT_EQ(31, date.day());  

	EXPECT_TRUE(date.setDate(2017, 2, 20));
	EXPECT_EQ(2017, date.year());
	EXPECT_EQ(2, date.month());
	EXPECT_EQ(20, date.day());  
}

TEST(DateTest, toIsoString){
	Date date(2018, 1, 31);

	EXPECT_STREQ("2018-01-31", date.toIsoString().c_str());  
}

TEST(DateTest, toString){
	Date date(2018, 1, 31);

	EXPECT_STREQ("20180131", date.toString().c_str());  
}

TEST(DateTest, isValid1){
	EXPECT_TRUE(Date::isValid(2018, 1, 1));  
	EXPECT_TRUE(Date::isValid(2018, 1, 31));  
	EXPECT_TRUE(Date::isValid(2018, 2, 28));  
	EXPECT_TRUE(Date::isValid(2020, 2, 29));  

	EXPECT_FALSE(Date::isValid(2018, 1, -1));  
	EXPECT_FALSE(Date::isValid(2018, 1, 32));  
	EXPECT_FALSE(Date::isValid(2018, 2, 29));  
}

TEST(DateTest, isLeapYear){
	EXPECT_TRUE(Date::isLeapYear(-5));  
	EXPECT_TRUE(Date::isLeapYear(-1));  
	EXPECT_TRUE(Date::isLeapYear(4));  
	EXPECT_TRUE(Date::isLeapYear(8));  

	EXPECT_FALSE(Date::isLeapYear(100));  
	EXPECT_FALSE(Date::isLeapYear(200));  
	EXPECT_FALSE(Date::isLeapYear(500));  
}


