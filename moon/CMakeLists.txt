file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/moon)

file(GLOB CORE_SOURCES_BASE "*.cpp")
file(GLOB CORE_SOURCES_DB "database/*.cpp")
file(GLOB CORE_SOURCES_IO "io/*.cpp")
file(GLOB CORE_SOURCES_LOGGER "logger/*.cpp")
file(GLOB CORE_SOURCES_NET "net/*.cpp")
file(GLOB CORE_SOURCES_OS "os/*.cpp")
file(GLOB CORE_SOURCES_THREAD "thread/*.cpp")


set(CORE_SOURCES 
    ${CORE_SOURCES_BASE}  
    ${CORE_SOURCES_DB}  
    ${CORE_SOURCES_IO}
    ${CORE_SOURCES_LOGGER} 
    ${CORE_SOURCES_NET} 
    ${CORE_SOURCES_OS} 
    ${CORE_SOURCES_THREAD} 
    )

add_library(moon ${CORE_SOURCES})
TARGET_LINK_LIBRARIES(moon pthread rt)

# find mysql library
SET(MYSQL_NAMES mysqlclient mysqlclient_r)
FIND_LIBRARY(MYSQL_LIBRARY NAMES ${MYSQL_NAMES} PATHS /usr/lib /usr/lib64)
if(MYSQL_LIBRARY)
    MESSAGE(STATUS "MYSQL_LIBRARY FOUND IS " ${MYSQL_LIBRARY})
    TARGET_LINK_LIBRARIES(moon ${MYSQL_LIBRARY})
else()
    MESSAGE(FATAL_ERROR "MYSQL_LIBRARY NOT FOUND" )
endif()

# install library
install(TARGETS moon DESTINATION lib)


add_subdirectory(io)
add_subdirectory(logger)
add_subdirectory(net)
add_subdirectory(os)
add_subdirectory(thread)
