/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_IO_WRITABLEFILE_H_
#define MOON_IO_WRITABLEFILE_H_

#include <moon/Status.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>  

#include <string>

namespace moon
{
class Slice;
class WritableFile;

typedef boost::shared_ptr<WritableFile> WritableFilePtr;

class WritableFile : boost::noncopyable
{
public:
	WritableFile();
	virtual ~WritableFile();

	virtual Status append(const Slice& data) = 0;
    virtual Status close() = 0;
    virtual Status flush() = 0;
public:
	static WritableFilePtr getDefaultWritableFile(const std::string &filepath);
};  // ~WritableFile


}  // ~moon

#endif