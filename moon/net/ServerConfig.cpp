/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/net/ServerConfig.h>
#include <moon/Logger.h>
#include <moon/Properties.h>

#include <errno.h>
#include <string.h>

namespace moon
{
namespace net
{

static const int kIoThreadNumMax = 100;

bool ServerConfig::loadConfig(const std::string &configPath, ServerConfig &serverConfig)
{
	Properties property;
    if (!property.load(configPath)) {
    	LOGGER_ERROR("load config:%s failed, error:%s", configPath.c_str(), strerror(errno));
        return false;
    }

    serverConfig.mHostname =  property.getProperty("hostname");
    serverConfig.mPort = static_cast<uint16_t>(property.getInt("port"));
    serverConfig.mIOThreadNum = property.getInt("iothread.num");
    serverConfig.mName =  property.getProperty("name");

    return serverConfig.revise();
}

bool ServerConfig::revise()
{
    if (mHostname.empty()) {
        mHostname = "0.0.0.0";
    }
    if (mPort == 0) {
    	LOGGER_ERROR("invalid server port:%d", mPort);
        return false;
    }
    if ( (mIOThreadNum <= 0) || (mIOThreadNum >= kIoThreadNumMax) )  {
        LOGGER_ERROR("invalid server oithread num:%d", mIOThreadNum);
        return false;
    }
    if (mName.empty()) {
        mName = "server";
    }

    return true;
}

}  // ~net
}  // ~moon