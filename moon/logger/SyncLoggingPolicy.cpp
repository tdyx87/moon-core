#include <moon/logger/LoggingPolicy.h>
#include <moon/logger/LoggerBuffer.h>

#include <stdio.h>

namespace moon
{

class SyncLoggingPolicy : public LoggingPolicy
{
public:
	void logging(const LoggerBufferPtr &buffer)
	{
	    fwrite(buffer->data(), 1, buffer->size(), stdout);
	    fflush(stdout);
	}
};

LoggingPolicyPtr LoggingPolicy::getSyncLoggingPolicy()
{
    return LoggingPolicyPtr(new SyncLoggingPolicy());
}

}  // ~moon

