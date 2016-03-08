#!/bin/bash

#
# Build script for Html5 Host
# Usage: Run from Moai SDK's root directory:
#
# ./bin/build-html.sh
#
# You can change the CMake options using -DOPTION=VALUE
# Check moai-dev/cmake/CMakeLists.txt for all the available options.
#
# The modules that are set in FALSE are not working or tested
#
# Please, check cmake/[module] for more info.
#
# Thanks!

: ${EMSCRIPTEN?"EMSCRIPTEN is not defined. Please set to the location of your emscripten install (path)"}

cd `dirname $0`/..
MOAIROOT=$(pwd)

mkdir -p build/build-html
cd build/build-html

cmake \
-DEMSCRIPTEN_ROOT_PATH=${EMSCRIPTEN} \
-DCMAKE_TOOLCHAIN_FILE=${EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake \
-DBUILD_HTML=TRUE \
-DCMAKE_BUILD_TYPE=Release \
-DMOAI_CHIPMUNK=FALSE \
-DMOAI_CURL=FALSE \
-DMOAI_CRYPTO=FALSE \
-DMOAI_LIBCRYPTO=FALSE \
-DMOAI_EXPAT=FALSE \
-DMOAI_MONGOOSE=FALSE \
-DMOAI_OGG=FALSE \
-DMOAI_OPENSSL=FALSE \
-DMOAI_SQLITE3=FALSE \
-DMOAI_SFMT=FALSE \
-DMOAI_VORBIS=FALSE \
-DMOAI_WEBP=FALSE \
-DMOAI_HTTP_CLIENT=FALSE \
-DMOAI_LUAJIT=FALSE \
$MOAIROOT/cmake/hosts/host-html

if [[ $? -ne 0 ]]; then
    exit 1
fi

make moaijs

if [[ $? -ne 0 ]]; then
  exit 1
fi

mkdir -p $MOAIROOT/lib/html
cp moaijs.js $MOAIROOT/lib/html/moaijs.js

