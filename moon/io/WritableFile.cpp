#include <moon/io/WritableFile.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <algorithm>

namespace moon
{
WritableFile::WritableFile()
{
}

WritableFile::~WritableFile()
{
}

static const size_t kBufSize = 65536;

class PosixWritableFile : public WritableFile
{
private:
	int mFd;
	std::string mFilepath;
	char mBuf[kBufSize];
	size_t mPos;
public:
	PosixWritableFile(int fd, const std::string &filepath) : mFd(fd), mFilepath(filepath)	{
	}

	~PosixWritableFile() {
	    if (mPos >= 0) {
            // Ignoring any potential errors
            this->close();
        }
	}
	
	virtual Status append(const Slice& data) 	{
	    if (data.empty()) {
			return Status::ok();
	    }
		
	    size_t n = data.size();
		const char *p = data.data();

		// Fit as much as possible into buffer.
        size_t copy = std::min(n, kBufSize - mPos);
		memcpy(mBuf + mPos, p, copy);
		p += copy;
		n -= copy;
		mPos += copy;
		if (0 == n) {
			return Status::ok();
		}

		// Can't fit in buffer, so need to do at least one write.
        Status s = flush();
        if (!s.ok()) {
            return s;
        }

		// Small writes go to buffer, large writes are written directly.
	    if (n < kBufSize) {
	        memcpy(mBuf, p, n);
	        mPos = n;
	        return Status::ok();
	    }
	    return this->writeRaw(p, n);
	}
	
    virtual Status close()	{
	    Status result = flush();
		const int ret = ::close(mFd);
		if ( ret < 0 && result.isOk()) {
			result = Status::posixError(errno, mFilepath);
		}
		mFd = -1;
		return result;
    }
	
    virtual Status flush()    {
        Status s = writeRaw(mBuf, mPos);
		mPos = 0;
		return s;
    }
private:
    Status writeRaw(const char* p, size_t n) {
		while (n > 0) {
			ssize_t r = ::write(mFd, p, n);
			if (r < 0) {
				if (EINTR == errno) {
					continue;
				}
				return Status::posixError(errno, mFilepath);
			}
			p += r;
			n -= r;
		}  // ~while

		return Status::ok();
    }
};

WritableFilePtr WritableFile::getDefaultWritableFile(const std::string &filepath)
{
    int fd = open(filepath.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0) {
		return WritableFilePtr();
	}
    return WritableFilePtr(new PosixWritableFile(fd, filepath));
}

}