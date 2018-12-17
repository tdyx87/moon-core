/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_BYTEBUFFER_H_
#define MOON_BYTEBUFFER_H_

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include <string>
#include <vector>

namespace moon
{

class ByteBuffer
{
public:
    ByteBuffer();
    ~ByteBuffer();
    
    /**
      Writes the bytes pointed by @data to the current write position and 
      increases the write position by the number of bytes written(that is strlen(data)).

      @params data: A pointer pointed to the data will be written to this object, if data is NULL, returns 0.
      @return Returns an non-negative number indicated the bytes actually written, on failure returns 0.
    */
    size_t append(const char *data);

    /**
      Writes @count bytes pointed by @data started at @count to the current write position and 
      increases the write position by count if success.

      @params data: A pointer pointed to the data will be written to this object, if data is NULL, returns 0.
      @params count: The number of bytes will be written, (count + offset) must not be large than strlen(data).
      @params offset: The offset with the data of the first byte to be written, must not be large and equal than strlen(data).
      
      @return Returns an non-negative number indicated the bytes actually written, on failure returns 0.
    */
    size_t append(const char *data, size_t count, size_t offset = 0);

    /** Writes the first one byte into this object and increases the write positions by 1, and returns 1. */
    size_t appendInt8(int i);

    /** Writes the first two bytes into this object and increases the write positions by 2, and returns 2. */
    size_t appendInt16(int i);

    /** Writes the first four byte into this object and increases the write positions by 4, and returns 4. */
    size_t appendInt32(int i);

    /** Writes the first eight bytes into this object and increases the write positions by 8, and returns 8. */
    size_t appendInt64(int64_t i);

    /** Writes the bytes in @l into this object and increases the write positions by sizeof(long), and returns sizeof(long). */
    size_t appendLong(long l);

    /** Writes @data to this object and increases the write positions by data.length(), and returns data.length(). */
    size_t append(const std::string &data);
    
    /** Clear all the bytes in this object. */
    void clear();
    
    /** Returns a pointer point the first bytes that should be read next time. */
    char* data() {return this->begin() + mReadIndex;}
    
    /** Returns a pointer point the first bytes that should be read next time. */
    const char* data() const {return this->begin() + mReadIndex;}
    
    /**  
      Reads at most @count bytes to the buffer, if readableBytes() is less than the @count, 
      reads readableBytes() bytes to the buffer, and increases the read position by the bytes actually read.

      @returns Returns an non-negative number indicated the bytes has been read.
    */
    size_t get(char *buffer, size_t count);

    /** 
      Reads a byte at the current read position as the return value, and increases the read position by one.
    */
    int getInt8();

    /** 
      Reads the two bytes at the current read position as the return value, and increases the read position by two.
    */
    int getInt16();

    /** 
      Reads the four bytes at the current read position as the return value, and increases the read position by four.
    */
    int getInt32();

    /** 
      Reads the eight bytes at the current read position as the return value, and increases the read position by eight.
    */
    int64_t getInt64();

    /** 
      Reads the sizeof(long) bytes at the current read position as the return value, and increases the read position by sizeo(long).
    */
    long getLong();

    /**
      Read the first len bytes as the return value, and increases the read position by len.
      if len large than readableBytes(), returns an empty string.
    */
    std::string getString(size_t len);

	  std::string getAllAsString()
	  {
		    return std::string(begin(), mBuffer.size());
	  }

    bool moveWriteIndexBy(size_t num);
    
    void swap(ByteBuffer& rhs)
	  {
	      mBuffer.swap(rhs.mBuffer);
	      std::swap(mReadIndex, rhs.mReadIndex);
	      std::swap(mWriteIndex, rhs.mWriteIndex);
	  }

    /** 返回当前可读字节数 */
	  size_t readableBytes() const 
	  {
		    assert(mWriteIndex >= mReadIndex);
		    return mWriteIndex - mReadIndex;
	  }

    void retrieve(size_t len)
    {
        if (len < this->readableBytes()) {
            mReadIndex += len;
        } else {
            this->clear();
        }
    }
	
	/** 返回当前可写字节数 */
	size_t writableBytes() const 
	{
		assert(mBuffer.size() >= mWriteIndex);
	    return mBuffer.size() - mWriteIndex;
	}
    
    void debugPrint()
    {
        printf("Buffer-size:%zu  mReadIndex:%zu  mWriteIndex:%zu\n", mBuffer.size(), mReadIndex, mWriteIndex);
    }

private:
    size_t appendInternal(const char *data, size_t count, size_t offset = 0);
	char* beginRead() {return this->begin() + mReadIndex;}
    const char* beginRead() const {return this->begin() + mReadIndex;}
	
	char* beginWrite() {return this->begin() + mWriteIndex;}
    const char* beginWrite() const {return this->begin() + mWriteIndex;}

	char* begin() { return &*mBuffer.begin();}
	const char* begin() const {return &*mBuffer.begin();}

	void hasWritten(size_t len) { mWriteIndex += len;}	

	void ensureWritableBytes(size_t len);

	void makeSpace(size_t len);
private:
    std::vector<char> mBuffer;
    size_t mReadIndex;
    size_t mWriteIndex;
};  // ~ByteBuffer

}  // ~moon
#endif