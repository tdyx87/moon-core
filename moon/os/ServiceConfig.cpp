/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/os/ServiceConfig.h>
#include <moon/Logger.h>
#include <moon/Properties.h>

#include <errno.h>
#include <string.h>

namespace moon
{
static const int kIoThreadNumMax = 100;

bool ServiceConfig::loadConfig(const std::string &configPath, ServiceConfig &serviceConfig)
{
	Properties property;
    if (!property.load(configPath)) {
    	LOGGER_ERROR("load config:%s failed, error:%s", configPath.c_str(), strerror(errno));
        return false;
    }

    serviceConfig.mServiceHostname =  property.getProperty("service.hostname");
    serviceConfig.mServicePort = static_cast<uint16_t>(property.getInt("service.port"));
    serviceConfig.mIOThreadNum = property.getInt("service.iothread.num");
    serviceConfig.mServiceName =  property.getProperty("service.name");

    return serviceConfig.revise();
}

bool ServiceConfig::revise()
{
    if (mServiceHostname.empty()) {
        mServiceHostname = "0.0.0.0";
    }
    if (mServicePort == 0) {
    	LOGGER_ERROR("invalid service port:%d", mServicePort);
        return false;
    }
    if ( (mIOThreadNum <= 0) || (mIOThreadNum >= kIoThreadNumMax) )  {
        LOGGER_ERROR("invalid service oithread num:%d", mIOThreadNum);
        return false;
    }
    if (mServiceName.empty()) {
        mServiceName = "service";
    }

    return true;
}

}  // ~moon