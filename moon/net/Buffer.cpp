/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/net/Buffer.h>
#include <moon/net/SocketOps.h>

#include <stdio.h>
#include <string.h>


namespace moon
{
namespace net
{
const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;
const char Buffer::kCRLF[] = "\r\n";

Buffer::Buffer() : mBuffer(kCheapPrepend + kInitialSize)
{
	mReadIndex = kCheapPrepend;
	mWriteIndex = kCheapPrepend;	
}

Buffer::~Buffer()
{
}

/** 读数据 */
int Buffer::readFd(int fd)
{
	char extrabuf[16382];
	struct iovec vec[2];
	const size_t writable = this->writableBytes();  // 获取可写入的字节数

	vec[0].iov_base = this->begin() + mWriteIndex;
	vec[0].iov_len = writable;
	vec[1].iov_base = extrabuf;
	vec[1].iov_len = sizeof (extrabuf);

	const int iRead = static_cast<int>(sockets::readv(fd, vec, 2));
	if (iRead <= 0) {
	    return iRead;
	} else if (iRead <= static_cast<int>(writable)) {
		this->hasWritten(iRead);   // has written iRead bytes		
	} else {
		mWriteIndex = mBuffer.size();
		this->append(extrabuf, iRead - writable);
	}

	return iRead;
}


/** 设置数据 */
void Buffer::append(const char* data, size_t len)
{
	this->ensureWritableBytes(len);
    std::copy(data, data+len, this->beginWrite());
	this->hasWritten(len);  
}

void Buffer::ensureWritableBytes(size_t len)
{
	if (this->writableBytes() < len) {
		this->makeSpace(len);
	}

	assert(this->writableBytes() >= len);
}


void Buffer::makeSpace(size_t len)
{
	bool isExpansion = (this->writableBytes() + this->prependableBytes()) < (len + kCheapPrepend);
	if (isExpansion) {
		// expand the size
		mBuffer.resize(mWriteIndex + len);
	} else {
		assert(kCheapPrepend < mReadIndex);
        
		// move readable data to the front
		size_t readable = this->readableBytes();
		std::copy(this->begin() + mReadIndex, this->begin() + mWriteIndex, this->begin() + kCheapPrepend);

		mReadIndex = kCheapPrepend;
        mWriteIndex = mReadIndex + readable;

        assert(readable == this->readableBytes());
	}
}

}  // ~net
}  // ~moon