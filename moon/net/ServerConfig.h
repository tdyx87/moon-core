/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#ifndef MOON_NET_SERVERCONFIG_H_
#define MOON_NET_SERVERCONFIG_H_

#include <string>

namespace moon
{
namespace net
{

class ServerConfig
{
public:
    static bool loadConfig(const std::string &configPath, ServerConfig &serverConfig);

	  uint16_t port() const {return mPort;}
	  int ioThreadNum() const {return mIOThreadNum;}
	  const std::string& hostname() const {return mHostname;}
	  const std::string& name() const {return mName;}

private:
	  bool revise();
private:
    uint16_t mPort;
    int mIOThreadNum;
    std::string mHostname;
    std::string mName;

};  // ~ServerConfig


}  // ~net
}  // ~moon
#endif