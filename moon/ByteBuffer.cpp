/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/ByteBuffer.h>

#include <string.h>
#include <algorithm>

namespace moon
{

ByteBuffer::ByteBuffer()
{
    mReadIndex = 0;
    mWriteIndex = 0;
}

ByteBuffer::~ByteBuffer()
{

}

size_t ByteBuffer::append(const char *data, size_t count, size_t offset)
{
	if ( (data == NULL) || (count == 0) ) {
        return 0;
	}
    if ( (offset >= strlen(data)) || (offset + count > strlen(data)) ){
        return 0;
    }

    return appendInternal(data, count, offset);
}

size_t ByteBuffer::append(const char *data)
{
	if (data == NULL) {
        return 0;
	}
    return appendInternal(data, strlen(data));
}

size_t ByteBuffer::appendInternal(const char *data, size_t count, size_t offset)
{
    assert(data != NULL);
    assert(strlen(data) >= (count + offset));

    this->ensureWritableBytes(count);
    std::copy(data + offset, data + count + offset, this->beginWrite());
    this->hasWritten(count); 
    return count;
}

size_t ByteBuffer::appendInt8(int i)
{
	int8_t num = static_cast<int8_t>(i);
    return appendInternal(reinterpret_cast<const char *>(&num), 1);
}

size_t ByteBuffer::appendInt16(int i)
{
    int16_t num = static_cast<int16_t>(i);
    return appendInternal(reinterpret_cast<const char *>(&num), 2);
}

size_t ByteBuffer::appendInt32(int i)
{
    int32_t num = static_cast<int32_t>(i);
    return appendInternal(reinterpret_cast<const char *>(&num), 4);
}

size_t ByteBuffer::appendInt64(int64_t i)
{
    return appendInternal(reinterpret_cast<const char *>(&i), 8);
}

size_t ByteBuffer::appendLong(long l)
{
    return appendInternal(reinterpret_cast<const char *>(&l), sizeof(long));
}

size_t ByteBuffer::append(const std::string &data)
{
    return this->appendInternal(data.c_str(), data.length());
}

void ByteBuffer::clear()
{
    mReadIndex = 0;
    mWriteIndex = 0;
    mBuffer.clear();
}

void ByteBuffer::ensureWritableBytes(size_t len)
{
    if (this->writableBytes() < len) {
		this->makeSpace(len);
	}
	assert(this->writableBytes() >= len);
}

size_t ByteBuffer::get(char *buffer, size_t count)
{
    if ( (buffer == NULL) || (count == 0) ) {
        return 0;
    }

    if (readableBytes() < count) {
        return 0;
    }

    std::copy(beginRead(), beginRead() + count, buffer);
    mReadIndex += count;
    return count;
}

int ByteBuffer::getInt8()
{
    if (readableBytes() < 1) {
        return 0;
    }
    return mBuffer[mReadIndex++];
}

int ByteBuffer::getInt16()
{
    if (readableBytes() < 2) {
        return 0;
    }

    int16_t num = 0;
    get(reinterpret_cast<char*>(&num), 2);
    return num;
}

int ByteBuffer::getInt32()
{
    if (readableBytes() < 4) {
        return 0;
    }

    int32_t num = 0;
    get(reinterpret_cast<char*>(&num), 4);
    return num;
}

int64_t ByteBuffer::getInt64()
{
    if (readableBytes() < 8) {
        return 0;
    }

    int64_t num = 0;
    get(reinterpret_cast<char*>(&num), 8);
    return num;
}

long ByteBuffer::getLong()
{
    if (readableBytes() < sizeof(long)) {
        return 0;
    }

    long num = 0;
    get(reinterpret_cast<char*>(&num), sizeof(long));
    return num;
}

std::string ByteBuffer::getString(size_t len)
{
	if (readableBytes() < len) {
        return "";
    }
    std::string result(this->data(), len);
    mReadIndex += len;
    return result;
}

void ByteBuffer::makeSpace(size_t len)
{
	bool isExpansion = this->writableBytes() < len;
	if (isExpansion) {
		// expand the size
		mBuffer.resize(mWriteIndex + len);
	} else {    
		// move readable data to the front
		size_t readable = this->readableBytes();
		std::copy(this->begin() + mReadIndex, this->begin() + mWriteIndex, this->begin());

		mReadIndex = 0;
        mWriteIndex = mReadIndex + readable;
        assert(readable == this->readableBytes());
	}
}

bool ByteBuffer::moveWriteIndexBy(size_t num)
{
    if (mBuffer.capacity() < num) {
        return false;
    }
    mWriteIndex += num;
    return true;
}

}  // ~moon