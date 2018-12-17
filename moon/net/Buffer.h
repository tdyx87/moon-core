/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_NET_BUFFER_H_
#define MOON_NET_BUFFER_H_


#include <assert.h>

#include <algorithm>
#include <vector>
#include <string>

namespace moon
{
namespace net
{

class Buffer
{
	static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;
public:
	Buffer();
    ~Buffer();

	int readFd(int fd);

	void swap(Buffer& rhs)
	{
	    mBuffer.swap(rhs.mBuffer);
	    std::swap(mReadIndex, rhs.mReadIndex);
	    std::swap(mWriteIndex, rhs.mWriteIndex);
	}
    
	// 返回当前缓冲区可读字节数
	size_t readableBytes()const 
	{
		return mWriteIndex - mReadIndex;
	}
	
	// 返回当前缓冲区可写字节数
	size_t writableBytes()const 
	{
	    return mBuffer.size() - mWriteIndex;
	}

	size_t prependableBytes()const {return mReadIndex;}
    
	// 返回指向当前缓冲区第一个可读字节的指针
	//const char* peekFirst()const 
	//{
	//	return data(0);
	//}
    
	const char* data() const
	{
		return this->begin() + mReadIndex;
	}
    
	// 获取len字节的数据
	// 如果len<readableBytes()获取len字节的数据,否则获取所有数据
	void retrieve(size_t len)
	{
		if (len < this->readableBytes()) {
			mReadIndex += len;
		} else {
			this->retrieveAll();
		}
	}

    // 获取所有数据
	void retrieveAll()
	{
		mWriteIndex = kCheapPrepend;
		mReadIndex = kCheapPrepend;
	}

	// 返回当前全部可读数据
	std::string retrieveAllAsString()
	{
		return this->retrieveAsString(this->readableBytes());
	}
    
	// 返回可读数据的前n个字节
	std::string retrieveAsString(size_t len)
	{
		assert(len <= this->readableBytes());

		std::string result(this->data(), len);
		this->retrieve(len);
		return result;
	}
    
	// 将指针@data所指字符数组中的前@len个字符拷贝到buffer中 
    void append(const char* data, size_t len);	
    
	/**
	  寻找缓冲区是否包含字符串@val，如果存在返回一个指针，指向@val第一次出现的地方，如果@val不存在，返回NULL
	*/
	const char* find(const std::string& val)const
	{
		const char* pos = std::search(this->begin(), this->beginWrite(), val.c_str(), val.c_str() + val.length());
		return (pos == this->beginWrite()) ? NULL : pos;
	}

private:    
	char* beginWrite() {return this->begin() + mWriteIndex;}
    const char* beginWrite()const {return this->begin() + mWriteIndex;}

	char* begin() {return &*mBuffer.begin();}
	const char* begin()const {return &*mBuffer.begin();}

	void hasWritten(size_t len){ mWriteIndex += len; }	

	void ensureWritableBytes(size_t len);

	void makeSpace(size_t len);

private:
	size_t mReadIndex;    
	size_t mWriteIndex; 
	std::vector<char> mBuffer;    

};




}  // ~net
}  // ~moon

#endif  // ~MOON_NET_BUFFER_H_