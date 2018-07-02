/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_STATUS_H_
#define MOON_STATUS_H_

#include <moon/Slice.h>

#include <string>

/**
  Status对象对一个操作接口进行封装，它可以表示操作成功的接口也可以表示操作失败的结果，
  并提供相应的错误信息。

  Status对象的const方法是线程安全的，非const方法不是线程安全的。
 */
namespace moon
{
class Slice;

class Status
{
public:	
	Status() :  mState(NULL) {}
	~Status();

	Status(const Status& rhs);
    Status& operator=(const Status& rhs);	

	static Status ok() {return Status();}
	
	static Status error(int code, const Slice& msg, const Slice& msg2=Slice()) {
	    return Status(code, msg, msg2);
	}
	static Status error(const Slice& msg){
	    return error(-1, msg);
	}

	static Status posixError(int code, const Slice& msg=Slice());

	operator bool() const {NULL == mState;}
	bool isOk() const {return NULL == mState;}

	int errorCode() const;

	const char* error() const;
	std::string errorString() const;
private:
	Status(int code, const Slice& msg, const Slice& msg2);
	static const char* copyState(const char* s);
private:
    /**
      OK status has a null mState.  Otherwise, mState is a new[] array of the following form:
      mState[0..3] == length of message
      mState[4]    == code
      mState[5..]  == message
    */
    const char *mState;

};  // ~Status

inline Status::Status(const Status& rhs) {
  mState = (rhs.mState == NULL) ? NULL : copyState(rhs.mState);
}

inline Status& Status::operator=(const Status& rhs) {
    // The following condition catches both aliasing (when this == &rhs),
    // and the common case where both rhs and *this are ok.
    if (mState != rhs.mState) {
        delete[] mState;
        mState = (rhs.mState == NULL) ? NULL : copyState(rhs.mState);
    }
    return *this;
}



}  // ~moon

#endif

