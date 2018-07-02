 /**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_SLICE_H_
#define MOON_SLICE_H_
 
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <string>

namespace moon
{

/**
  Slice包含了一个指向外部存储数据的指针，以及一个记录引用数据大小的整数。Slice的主要目的是方便处理一段字符数据。
  Slice对象只是引用了外部的一段数据，因此当被引用的外部数据被释放以后，就不能再操作Slice对象了。

  在多线程环境下，Slice对象的const方法都是线程安全的。Slice对象的非const方法不是线程安全的，
  在多线程环境下需要使用额外的线程同步机制。
*/
class Slice
{
public:
	Slice() : mData(NULL), mSize(0) {}

	Slice(const char *data) : mData(data), mSize(static_cast<size_t>(strlen(data))) {}
	
	Slice(const unsigned char *data) 
		: mData(reinterpret_cast<const char *>(data)), 
		  mSize(static_cast<size_t>(strlen(mData))) {}

	Slice(const std::string &s) : mData(s.c_str()), mSize(s.size()) {}

	Slice(const char *data, size_t size) : mData(data), mSize(size) {}

	/**
	  返回一个const char*指针，指向Slice引用的数据的起始位置。	  当Slice对象为空时，返回NULL。
	  该方法返回的数据有可能不包含'\0'字符（null terminated）。因此在期望有'\0'结尾场景，
	  不要将data()传入。如果真需要这么做，可以使用toString().c_str()来代替。
	*/
	const char* data() const {return mData;}

	// 返回Slice引用数据的字节数，如果Slice为空，则返回0.
	size_t size() const {return mSize;}

	// 返回true表示该对象没有引用任何数据
	bool empty() const {return 0 == mSize;}

	char operator[] (size_t n) const {
	    assert(n < size());
	    return mData[n];
	}

	void clear() {mData = NULL; mSize = 0;}

	void set(const void *buffer, size_t size) {
	    mData = reinterpret_cast<const char*>(buffer);
		mSize = size;
	}

	// 屏蔽Slice对象对前n个字节进行访问。
	void removePrefix(size_t n) {
	    assert(n <= size());
		mData += n;
		mSize -= n;
	}

	// 屏蔽Slice对象对后n个字节进行访问。
	void removeSuffix(size_t n) {
	    assert(n <= size());
		mSize -= n;
	}

	// 将Slice当前引用的数据以std::string形式返回。
	std::string toString() const {return std::string(mData, mSize);}

	bool startsWith(const Slice &b) const {
	     return (mSize >= b.size()) && (memcmp(mData, b.data(), b.size()) == 0);
	}

	/**
	  如果当前对象小于b返回-1；
	  如果当前对象等于b返回0；
	  如果当前对象大于b返回1；
	*/
	int compare(const Slice& b) const;

#define SLICE_BINARY_PREDICATE(cmp, auxcmp)                                            \
    bool operator cmp (const Slice &rhs) {                                             \
        int r = memcmp(data(), rhs.data(), size() < rhs.size() ? size() : rhs.size()); \
        return ( (r auxcmp 0) || ((r == 0) && ( size() cmp rhs.size()) ) ); 	       \
    } 

	SLICE_BINARY_PREDICATE(<, <);
	SLICE_BINARY_PREDICATE(<=, <);
	SLICE_BINARY_PREDICATE(>, >);
	SLICE_BINARY_PREDICATE(>=, >);
#undef SLICE_BINARY_PREDICATE
private:
    const char *mData;
	size_t mSize;
};  // ~Slice

inline int Slice::compare(const Slice& b) const {
    const size_t minLen = (mSize < b.mSize) ? mSize : b.mSize;
    int r = memcmp(mData, b.mData, minLen);
    if (r == 0) {
        if (mSize < b.mSize) {
			return -1;
        }
        else if (mSize > b.mSize) {
			return 1;
        }
    }
    return r;
}

inline bool operator==(const Slice &lhs, const Slice &rhs)  
{
    return (lhs.size() == rhs.size()) && (memcmp(lhs.data(), rhs.data(), lhs.size()) == 0);
}

inline bool operator!=(const Slice &lhs, const Slice &rhs)  
{
    return !(lhs == rhs);
}


} // ~moon

#endif  // ~end of file
