#include <moon/net/ServerLoader.h>
#include <moon/net/ServerConfig.h>
#include <moon/net/InetAddress.h>
#include <moon/net/TcpServer.h>
#include <moon/os/EventLoop.h>
#include <moon/Logger.h>

namespace moon
{
namespace net
{

ServerLoader& ServerLoader::getInstance()
{
	static ServerLoader instance;
	return instance;
}

ServerLoader::ServerLoader()
{
}

int ServerLoader::load(EventLoop *loop, const std::string &configPath)
{
    ServerConfig config;
    if (!ServerConfig::loadConfig(configPath, config)) {
    	LOGGER_ERROR("load server config failed");
        return -1;
    }
    
    InetAddress listenAddr(config.hostname().c_str(), config.port());
    mServer = TcpServerPtr(new TcpServer(loop, listenAddr, config.name()));

    mServer->setThreadNum(config.ioThreadNum());
    return 0;
}

TcpServerPtr ServerLoader::getServer()
{
	assert(mServer);
    return mServer;
}

}  // ~net
}  // ~moon

