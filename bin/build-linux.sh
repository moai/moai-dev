#!/bin/bash

#
# Build script for Mac OSX
# Usage: Run from Moai SDK's root directory:
#
# build-osx-sdl.sh
#
# You can change the CMake options using -DOPTION=VALUE
# Check moai-dev/cmake/CMakeLists.txt for all the available options.
#
if [ x$1 == x ]; then
  libprefix=`dirname $0`/../lib/linux
else
  libprefix=$1
fi
mkdir -p $libprefix

libprefix=$(cd $libprefix; pwd)

cd `dirname $0`/..

moai_root=$(pwd)


if ! [ -d "build" ]
then
mkdir build
fi
cd build

if ! [ -d "build-linux" ]
then
mkdir build-linux
fi
cd build-linux


set -e
cmake \
-DBUILD_LINUX=TRUE \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$libprefix \
$moai_root/cmake/hosts/host-linux-sdl

cmake --build . --target install

if [ ! -e "$moai_root/util/moai" ]; then
   cp $libprefix/bin/moai $moai_root/util/moai
fi