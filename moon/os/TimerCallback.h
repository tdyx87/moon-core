/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_OS_TIMERCALLBACK_H_
#define MOON_OS_TIMERCALLBACK_H_

#include <boost/function.hpp>

namespace moon {
    typedef boost::function<void()> TimerCallback;
}

#endif

