#!/bin/bash

#
# Build script for Mac OSX
# Usage: Run from Moai SDK's root directory:
#
# ./bin/build-osx.sh
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
-DCMAKE_BUILD_TYPE=Debug \
../

make -j8
if [[ $? -ne 0 ]]; then
    exit 1
fi

#
# Copy libs to lib
#
rm -rf ../../release/osx/x64/
mkdir -p ../../release/osx/x64/lib/
mkdir -p ../../release/osx/x64/bin/
for i in * ; do
  if [ -d "$i" ]; then
    if [ -f $i/lib$i.a ]; then
      echo "Copying $i/lib$i.a to release/osx/lib/x64"
      cp $i/lib$i.a ../../release/osx/x64/lib/
    fi
  fi
done
pwd
cp host-glut/moai ../../release/osx/x64/bin/
