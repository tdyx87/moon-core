#include <moon/Timestamp.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>

namespace moon {

std::string Timestamp::toFormattedString() const
{
    char buf[32] = {0};
    time_t seconds = static_cast<time_t>(mMicroSecondsSinceEpoch / kMicroSecondsPerSecond);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d",
        tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
        tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    return buf;
}

std::string Timestamp::toMicroFormattedString() const
{
	char buf[32] = {0};
    time_t seconds = static_cast<time_t>(mMicroSecondsSinceEpoch / kMicroSecondsPerSecond);
    int microseconds = static_cast<int>(mMicroSecondsSinceEpoch % kMicroSecondsPerSecond);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d.%06d",
        tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
        tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, microseconds);
    return buf;
}

Timestamp Timestamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

uint64_t Timestamp::currentMicroSeconds()
{
	struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t seconds = tv.tv_sec;

	return seconds * kMicroSecondsPerSecond + tv.tv_usec;
}

uint64_t Timestamp::currentMilliSeconds()
{
	struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t seconds = tv.tv_sec;

	return (seconds * kMicroSecondsPerSecond + tv.tv_usec) / kMilliSecondsPerSecond;
}

uint64_t Timestamp::currentSeconds()
{
	struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}



} // ~moon