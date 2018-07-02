/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_TYPECAST_H_
#define MOON_TYPECAST_H_


namespace moon
{

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
    return f;
}

}  // ~moon



#endif  // ~
