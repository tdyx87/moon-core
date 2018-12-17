/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_OS_SERVICECONFIG_H_
#define MOON_OS_SERVICECONFIG_H_

#include <string>

namespace moon
{

class ServiceConfig
{
public:
	static bool loadConfig(const std::string &configPath, ServiceConfig &serviceConfig);

	uint16_t servicePort() const {return mServicePort;}
	int ioThreadNum() const {return mIOThreadNum;}
	const std::string& serviceHostname() const {return mServiceHostname;}
	const std::string& serviceName() const {return mServiceName;}

private:
	bool revise();
private:
    uint16_t mServicePort;
    int mIOThreadNum;
    std::string mServiceHostname;
    std::string mServiceName;

};  // ~ServiceConfig


}  // ~moon


#endif