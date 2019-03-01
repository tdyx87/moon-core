#include <moon/net/ServerLoader.h>
#include <moon/net/ServerConfig.h>
#include <moon/net/InetAddress.h>
#include <moon/net/TcpServer.h>
#include <moon/core/EventLoop.h>
#include <moon/Logger.h>

namespace moon
{
namespace net
{

TcpServerPtr ServerLoader::load(EventLoop *loop, const std::string &configPath)
{
    ServerConfig config;
    if (!ServerConfig::loadConfig(configPath, config)) {
    	LOGGER_ERROR("load server config failed");
        return TcpServerPtr();
    }
    
    InetAddress listenAddr(config.hostname().c_str(), config.port());
    TcpServerPtr server(new TcpServer(loop, listenAddr, config.name()));

    server->setThreadNum(config.ioThreadNum());
    return server;
}

}  // ~net
}  // ~moon

