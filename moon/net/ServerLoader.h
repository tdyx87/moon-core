#ifndef MOON_NET_SERVERLOADER_H_
#define MOON_NET_SERVERLOADER_H_

# include <string>

namespace moon
{
namespace net
{

class ServerLoader
{
public:
	int load(const std::string &configPath);
};  //~ ServerLoader

}  //~ net
}  //~ moon

#endif