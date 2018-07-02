/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_ATOMICINTEGER_H_
#define MOON_ATOMICINTEGER_H_

#include <stdint.h>

namespace moon {

/**
  An AtomicInteger that support lock-free thread-safe programming on Number types.
  All operations in an AtomicInteger object is atomically.
*/
template <class T> class AtomicInteger
{
public:
	AtomicInteger() : mValue(T()) {}
	AtomicInteger(const T &t) : mValue(t) {}

	// Returns the current value.
	T get() {
		return __sync_add_and_fetch(&mValue, 0);
	}
	
	// Atomically add the given value by delta, and returns the previous value.
	T getAndAdd(T delta) {
		return __sync_fetch_and_add(&mValue, delta);
	}

	// Atomically add the given value by delta, and returns the updated value.
	T addAndGet(T delta) {
		return getAndAdd(delta) + delta;
	}

	// Atomically increments the current value by one, and returns the updated value.
	T incrementAndGet() {
		return addAndGet(1); 
	}

	// Atomically decrements the current value by one, and returns the updated value.
    T decrementAndGet() { 
		return addAndGet(-1);
	}

	// Atomically add the given value by delta.
    void add(T delta) {
		getAndAdd(delta);
	}

	// Atomically increments the given value by delta.
    void increment()     {
		incrementAndGet();
	}

	// Atomically decrements the given value by delta.
    void decrement()     { 
		decrementAndGet(); 
    }

	// Atomically sets to the given value, and returns the old value.
    T getAndSet(T newValue)    {
        return __sync_lock_test_and_set(&mValue, newValue);
    }
    
	// Atomically sets to the given value.
	void set(T newValue) {__sync_lock_test_and_set(&mValue, newValue);}
	
private:
	volatile T mValue;
};

typedef moon::AtomicInteger<int>     AtomicInt;
typedef moon::AtomicInteger<int8_t>  AtomicInt8;
typedef moon::AtomicInteger<int16_t> AtomicInt16;
typedef moon::AtomicInteger<int32_t> AtomicInt32;
typedef moon::AtomicInteger<int64_t> AtomicInt64;

}  // ~namespace moon


#endif
