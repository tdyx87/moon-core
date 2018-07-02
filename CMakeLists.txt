cmake_minimum_required(VERSION 2.6)

project(moon)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(NOT CMAKE_BUILD_OPTION)
    set(CMAKE_BUILD_OPTION "moon")
endif()

if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()

set(CXX_FLAGS
 -g
 # -DVALGRIND
 # -DMUDUO_STD_STRING
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 # -std=c++0x
 -rdynamic
 )

# set complie options
set(CMAKE_CXX_COMPILER "g++")
#set(CMAKE_CXX_COMPILER "icpc")
if(CMAKE_BUILD_TYPE STREQUAL "debug")
   #set(CMAKE_CXX_FLAGS_DEBUG "-O0") 
   message(STATUS "Project to be build at debug mode.")
   set("$ENV{CXX_FLAGS} -O0 -Wall -g -ggdb") 
elseif(CMAKE_BUILD_TYPE STREQUAL "release")
   message(STATUS "Project to be build at release mode.")
   set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
endif()


# set global variables
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

find_package(Boost REQUIRED)

# include the non-standard header file search path
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

# add the non-standard library search path
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib ${PROJECT_SOURCE_DIR}/libs /usr/local/lib)

message(STATUS "PROJECT_BINARY_DIR:" ${PROJECT_BINARY_DIR})

add_subdirectory(moon)
add_subdirectory(test)
add_subdirectory(example)

if (${CMAKE_BUILD_OPTION} STREQUAL "example")
    MESSAGE(STATUS "Build examples:")   
#    add_subdirectory(example)
elseif(${CMAKE_BUILD_OPTION} STREQUAL "all")
    MESSAGE(STATUS "Build all")

#    add_subdirectory(example)
  elseif(CMAKE_BUILD_OPTION)
    MESSAGE(FATAL "invalid build option:" ${CMAKE_BUILD_OPTION})
endif()

