/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_PROPERTIES_H_
#define MOON_PROPERTIES_H_

#include <map>
#include <string>

namespace moon
{

class Properties
{
public:
	int load(const std::string& fileName);

	// 返回true表示key对应的键存在
	bool containsKey(const std::string& key)const {return mProperties.count(key) == 1;}

	std::string getProperty(const std::string& name, const std::string defaultValue = "") const;
	int getInt(const std::string& name, int defaultValue) const;
	int getLong(const std::string& name, long defaultValue) const;
	int getDouble(const std::string& name, double defaultValue) const;
private:
	void parseLine(const std::string &line);
private:
	std::map<std::string, std::string> mProperties;
};  //~ Properties


}  //~ moon

#endif