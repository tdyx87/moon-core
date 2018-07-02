#ifndef MOON_TIMESTAMP_H_
#define MOON_TIMESTAMP_H_

#include <stdint.h>
#include <string>

namespace moon {

class Timestamp
{
public:
	static const int kMicroSecondsPerSecond = 1000 * 1000;
    static const int kMilliSecondsPerSecond = 1000;

	Timestamp() : mMicroSecondsSinceEpoch(0) {}
  
    explicit Timestamp(uint64_t microSecondsSinceEpoch) : mMicroSecondsSinceEpoch(microSecondsSinceEpoch) {}
    
	// 返回一个long整数，表示当前时间戳对象对应的微秒数(自1970年1月1号0点以后)
	uint64_t usecs() const {return mMicroSecondsSinceEpoch;}

	// 返回一个long整数，表示当前时间戳对象对应的毫秒数(自1970年1月1号0点以后)
	uint64_t msecs() const {return mMicroSecondsSinceEpoch / kMilliSecondsPerSecond;}

	// 返回一个long整数，表示当前时间戳对象对应的秒数(自1970年1月1号0点以后)
	uint64_t secs() const {return mMicroSecondsSinceEpoch / kMicroSecondsPerSecond;}
    
	// 返回当前时间戳的字符表达形式，格式:yyyy-mm-dd hh:mm:ss
	std::string toFormattedString() const;

	// 返回当前时间戳的字符表达形式，格式:yyyy-mm-dd hh:mm:ss.ssssss。
	// 与toFormattedString的区别是该方法返回的字符串附加微秒数。
	std::string toMicroFormattedString() const;
    
	bool valid() const {return mMicroSecondsSinceEpoch > 0;}
    
	// 返回一个代表当前时间戳的Timestamp对象。
	static Timestamp now();

	// 返回从1970年1月1号0点以来的微秒数
    static uint64_t currentMicroSeconds();

	// 返回从1970年1月1号0点以来的毫秒数
    static uint64_t currentMilliSeconds();

	// 返回从1970年1月1号0点以来的秒数
    static uint64_t currentSeconds();

	/**
	  计算@high与@low之间差，返回结果单位为：秒
	*/
	static inline uint64_t diff(Timestamp high, Timestamp low)
	{
	    uint64_t diff = high.usecs() - low.usecs();
	    return diff / Timestamp::kMicroSecondsPerSecond;
	}

	/**
	  在当前timestamp对象上添加@seconds秒，然后返回一个新的@Timestamp对象表示相加后的结果。
	  @return 返回一个表示timestamp+seconds的对象
	*/
	static inline Timestamp addSecs(Timestamp timestamp, long secs)
	{
	    return Timestamp(timestamp.usecs() + secs * Timestamp::kMicroSecondsPerSecond);
	}

	static inline Timestamp addMsecs(Timestamp timestamp, long msecs)
	{
	    return Timestamp(timestamp.usecs() + msecs * Timestamp::kMilliSecondsPerSecond);
	}

	
	static inline Timestamp addUsecs(Timestamp timestamp, long usecs)
	{
		return Timestamp(timestamp.usecs() + usecs);
	}

	static Timestamp invalid()
    {
        return Timestamp();
    }

	static Timestamp createFromSeconds(uint64_t seconds)
	{
		return Timestamp(seconds * kMicroSecondsPerSecond);
	}

	static Timestamp createFromMillSeconds(uint64_t millSeconds)
	{
		return Timestamp(millSeconds * kMilliSecondsPerSecond);
	}
private:
	uint64_t mMicroSecondsSinceEpoch;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
  return lhs.usecs() < rhs.usecs();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
  return lhs.usecs() == rhs.usecs();
}



}  // ~moon
#endif  // ~TIMESTAMP_H_
