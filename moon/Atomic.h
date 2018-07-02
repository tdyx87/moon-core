/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_ATOMIC_H_
#define MOON_ATOMIC_H_

#include <stdint.h>

namespace moon {

template <class T> class Atomic
{
public:
	Atomic() : mValue(T()) {}
	Atomic(const T &t) : mValue(t) {}
    
	T get() {return __sync_add_and_fetch(&mValue, 0);}

	void set(const T &t) {__sync_lock_test_and_set(&mValue, t);}

	// 现有值增加delta，并返回操作之前的值。
	T fetchAndAdd(const T &delta) {return __sync_fetch_and_add(&mValue, delta);}

	// 现有值减少delta，并返回操作之前的值。
	T fetchAndSub(const T &delta) {return __sync_fetch_and_sub(&mValue, delta);}

	// 对当前值做按位做'或'运算，并返回操作之前的值。
	T fetchAndOr(const T &t) {return __sync_fetch_and_or(&mValue, t);}

	// 对当前值做按位做'与'运算，并返回操作之前的值。
	T fetchAndAnd(const T &t) {return __sync_fetch_and_and(&mValue, t);}

	// 对当前值做按位做'异或'运算，并返回操作之前的值。
	T fetchAndXor(const T &t) {return __sync_fetch_and_xor(&mValue, t);}



	// 现有值增加delta，并返回操作之后的值。
	T addAndFetch(const T &delta) {return __sync_add_and_fetch(&mValue, delta);}

	// 现有值减少delta，并返回操作之后的值。
	T subAndFetch(const T &delta) {return __sync_sub_and_fetch(&mValue, delta);}

	// 对当前值做按位做'或'运算，并返回操作之后的值。
	T orAndFetch(const T &t) {return __sync_or_and_fetch(&mValue, t);}

	// 对当前值做按位做'与'运算，并返回操作之后的值。
	T andAndFetch(const T &t) {return __sync_and_and_fetch(&mValue, t);}

	// 对当前值做按位做'异或'运算，并返回操作之后的值。
	T xorAndFetch(const T &t) {return __sync_xor_and_fetch(&mValue, t);}


    // 如果当前值为expect，则将值设置为newval，并返回true。如果当前值不为expect，不改变当前值并返回false.
	bool compareAndSet(const T &expect, const T &newval)
	{
		return __sync_bool_compare_and_swap(&mValue, expect, newval);
	}
	
private:
	T mValue;
};

typedef Atomic<int>      atomic_t;
typedef Atomic<int8_t>   atomic8_t;
typedef Atomic<int16_t>  atomic16_t;
typedef Atomic<int32_t>  atomic32_t;
typedef Atomic<int64_t>  atomic64_t;
typedef Atomic<uint8_t>  uatomic8_t;
typedef Atomic<uint16_t> uatomic16_t;
typedef Atomic<uint32_t> uatomic32_t;
typedef Atomic<uint64_t> uatomic64_t;

}  // ~namespace moon


#endif