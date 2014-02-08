#!/bin/bash

#
# Build script for BlackBerry 10
# Usage: Run from Moai SDK's root directory:
#
# ./bin/build-blackberry10
#
# You can change the CMake options using -DOPTION=VALUE
# Check moai-dev/cmake/CMakeLists.txt for all the available options.
#
# The modules that are set in FALSE are not working from our third party
# tree, but from BlackBerry's provided ports intead, or not working at all.
#
# Please, check cmake/[module] for more info.
#
# Thanks!

: ${QNX_HOST?"QNX_HOST is not defined. Please run 'source /path/to/bbndk-env.sh'"}

cd `dirname $0`/..
cd cmake
rm -rf build
mkdir build
cd build
PATH=$QNX_HOST/usr/bin:$PATH
CC=$QNX_HOST/usr/bin/cc CXX=$QNX_HOST/usr/bin/qcc cmake \
-DBUILD_BLACKBERRY=TRUE \
-DMOAI_BOX2D=TRUE \
-DMOAI_CHIPMUNK=TRUE \
-DMOAI_CURL=FALSE \
-DMOAI_CRYPTO=FALSE \
-DMOAI_EXPAT=TRUE \
-DMOAI_FREETYPE=TRUE \
-DMOAI_JSON=TRUE \
-DMOAI_JPG=TRUE \
-DMOAI_MONGOOSE=TRUE \
-DMOAI_OGG=TRUE \
-DMOAI_OPENSSL=FALSE \
-DMOAI_SQLITE3=TRUE \
-DMOAI_TINYXML=TRUE \
-DMOAI_PNG=TRUE \
-DMOAI_SFMT=TRUE \
-DMOAI_VORBIS=TRUE \
-DMOAI_HTTP_CLIENT=FALSE \
-DMOAI_UNTZ=TRUE \
../
if [[ $? -ne 0 ]]; then
    exit 1
fi

make -j4
if [[ $? -ne 0 ]]; then
  #
  # Since -j4 makes errors hard to read, 
  # we re-run make without -j to get the 
  # proper error.
  make
  exit 1
fi

#
# Copy libs to lib/blackberry
#
rm -rf ../../lib/blackberry
mkdir -p ../../lib/blackberry
for i in * ; do
  if [ -d "$i" ]; then
    if [ -f $i/lib$i.a ]; then
      echo "Copying $i/lib$i.a to lib/blackberry/"
      cp $i/lib$i.a ../../lib/blackberry
    fi
  fi
done
