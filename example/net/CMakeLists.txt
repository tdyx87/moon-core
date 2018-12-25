ADD_EXECUTABLE(example_net_tcpserver TcpServer.cxx)
TARGET_LINK_LIBRARIES(example_net_tcpserver moon)

ADD_EXECUTABLE(example_http_server1 HttpServerTest1.cxx)
TARGET_LINK_LIBRARIES(example_http_server1 moon)

#ADD_EXECUTABLE(example_net_server1 Server1Test.cxx)
#TARGET_LINK_LIBRARIES(example_net_server1 moon)

