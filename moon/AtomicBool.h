/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_ATOMICBOOL_H_
#define MOON_ATOMICBOOL_H_

namespace moon {

/**
  An AtomicBool object that maybe updated atomically, which support lock-free thread-safe programming.
*/
class AtomicBool
{
public:
	AtomicBool() : mFlag(0) {}

	bool get() {__sync_add_and_fetch(&mValue, 0) > 0;}

	bool getAndSet(bool newvalue) {__sync_lock_test_and_set(&mValue, newvalue ? 1 : 0) > 0;}
	
	void set(bool newvalue) {__sync_lock_test_and_set(&mValue, newvalue ? 1 : 0);}	
private:
	volatile int mValue;
};

}  // ~moon

#endif