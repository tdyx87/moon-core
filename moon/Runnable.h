/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_RUNNABLE_H_
#define MOON_RUNNABLE_H_

namespace moon
{

class Runnable
{
public:
    virtual ~Runnable() {}

    /// Run this runnable, must be implemented.
    virtual bool run() = 0;
};

}  // ~moon

#endif