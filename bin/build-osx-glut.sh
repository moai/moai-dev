#!/bin/bash

#
# Build script for Mac OSX
# Usage: Run from Moai SDK's root directory:
#
# build-osx-glut.sh
#
# You can change the CMake options using -DOPTION=VALUE
# Check moai-dev/cmake/CMakeLists.txt for all the available options.
#

cd `dirname $0`/..
cd cmake
rm -rf build
mkdir build
cd build
cmake \
-DBUILD_OSX=TRUE \
-DSDL_HOST=FALSE \
-DMOAI_BOX2D=TRUE \
-DMOAI_CHIPMUNK=TRUE \
-DMOAI_CURL=TRUE \
-DMOAI_CRYPTO=TRUE \
-DMOAI_EXPAT=TRUE \
-DMOAI_FREETYPE=TRUE \
-DMOAI_JSON=TRUE \
-DMOAI_JPG=TRUE \
-DMOAI_LUAEXT=TRUE \
-DMOAI_MONGOOSE=TRUE \
-DMOAI_OGG=TRUE \
-DMOAI_OPENSSL=TRUE \
-DMOAI_SQLITE3=TRUE \
-DMOAI_TINYXML=TRUE \
-DMOAI_PNG=TRUE \
-DMOAI_SFMT=TRUE \
-DMOAI_VORBIS=TRUE \
-DMOAI_UNTZ=TRUE \
-DMOAI_LUAJIT=TRUE \
-DMOAI_HTTP_CLIENT=TRUE \
-DCMAKE_BUILD_TYPE=Release \
../

make -j8
if [[ $? -ne 0 ]]; then
    exit 1
fi

#
# Copy libs to lib
#
rm -rf ../../release/osx/host-glut
mkdir -p ../../release/osx/host-glut/bin
mkdir -p ../../release/osx/host-glut/lib
find . -name "*.a" -print | xargs -J % cp -fp % ../../release/osx/host-glut/lib
cp host-glut/moai ../../release/osx/host-glut/bin
