#include <moon/Status.h>

#include <errno.h>
#include <stdint.h>

namespace moon
{

Status Status::posixError(int code, const Slice& msg)
{
    char errnobuf[1024] = {0};
    strerror_r(code, errnobuf, sizeof(errnobuf));
	
	Slice msg2(errnobuf);
	if (!msg.empty())
	{
	    return Status(code, msg, msg2);
	}

	return Status(code, Slice("error"), msg2);
}

const char* Status::copyState(const char *state) {
    uint32_t size;
    memcpy(&size, state, sizeof(size));
    char *result = new char[size + 6];
    memcpy(result, state, size + 6);
    return result;
}

Status::Status(int code, const Slice& msg, const Slice& msg2) {
    const uint32_t len1 = msg.size();
    const uint32_t len2 = msg2.size();
    const uint32_t size = len1 + (len2 ? (2 + len2) : 0);
    char *result = new char[size + 6];
    memcpy(result, &size, sizeof(size));
    result[4] = static_cast<char>(code);
    memcpy(result + 5, msg.data(), len1);
    if (len2) {
        result[5 + len1] = ':';
        result[6 + len1] = ' ';
        memcpy(result + 7 + len1, msg2.data(), len2);
    }
	
	result[size + 6 - 1] = '\0';
    mState = result;
}

Status::~Status()
{
    if (NULL != mState)
    {
        delete[] mState;
		mState = NULL;
    }
}

int Status::errorCode() const
{
    if (NULL == mState)
    {
        return 0;
    }

    return mState[4];
}

const char* Status::error() const
{
    if (NULL == mState)
    {
        return NULL;
    }

	return mState + 5;
}

std::string Status::errorString() const
{
    if (NULL == mState)
    {
        return "OK";
    }

	uint32_t length;
	memcpy(&length, mState, sizeof(length));

	return std::string(mState + 5);
}



}  // ~moon