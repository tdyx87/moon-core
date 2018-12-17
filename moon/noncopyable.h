/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NONCOPYABLE_H
#define MOON_NONCOPYABLE_H

namespace moon
{

class noncopyable
{
public:
    noncopyable(const noncopyable&);
    noncopyable& operator=(const noncopyable&);

protected:
    noncopyable(){}
    ~noncopyable() {}
};

}  //~ namespace moon

#endif  // MOON_NONCOPYABLE_H
