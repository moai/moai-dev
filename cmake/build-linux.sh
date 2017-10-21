#!/bin/bash

#
# Build script for GNU/Linux
# Usage: Run from Moai SDK's root directory:
#
# build-linux.sh
#
# You can change the CMake options using -DOPTION=VALUE
# Check moai-dev/cmake/CMakeLists.txt for all the available options.
#

set -e

if [ -z $1 ]; then
  libprefix=`dirname $0`/../lib/linux
else
  libprefix=$1
fi

mkdir -p $libprefix

libprefix=$(cd $libprefix; pwd)
cores=$(getconf _NPROCESSORS_ONLN)

: ${MOAI_SDK_HOME?"MOAI_SDK_HOME is not defined. Please set to the location of your MOAI SDK install (path)"}

pushd `dirname $0`/..
PITO_ROOT=$(pwd)


BUILD_DIR="build/build-linux"

if ! [ -d ${BUILD_DIR} ]; then
	mkdir -p $BUILD_DIR
fi

cd $BUILD_DIR

# - This fix, "curl: sed not found in PATH. Cannot continue without sed."
# - Shells problems: fish,
if ! [ -e PATH_SEPARATOR ]; then
	export PATH_SEPARATOR=:
fi

cmake \
-DBUILD_LINUX=TRUE \
-DMOAI_SDL=TRUE \
-DMOAI_SDK_HOME=${MOAI_SDK_HOME} \
-DMOAI_HTTP_SERVER=TRUE \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$libprefix \
$PITO_ROOT/cmake/hosts/host-linux-sdl

cmake --build . --target install -- -j$cores

if [ ! -e "${PITO_ROOT}/bin/moai" ]; then
   cp $libprefix/bin/moai $PITO_ROOT/bin/moai
fi

if [ ! -e "${MOAI_SDK_HOME}/util/moai" ]; then
   cp $libprefix/bin/moai ${MOAI_SDK_HOME}/util/moai
fi

popd
exit 0
