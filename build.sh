#!/bin/sh

set -x

SOURCE_DIR=`pwd`

# build directory
BUILD_DIR=${BUILD_DIR:-./build}

# build type, debug or release
BUILD_TYPE=${BUILD_TYPE:-debug}

# file installation directory
#INSTALL_DIR=${INSTALL_DIR:-./${BUILD_TYPE}-install}
INSTALL_DIR=${INSTALL_DIR:-/usr/local}

# set build options
BUILD_OPTION=$1

mkdir -p $BUILD_DIR/$BUILD_TYPE
cd $BUILD_DIR/$BUILD_TYPE 
cmake --graphviz=dep.dot \
      -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
      -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
      -DCMAKE_BUILD_OPTION=$BUILD_OPTION \
      $SOURCE_DIR \

make
#make install
