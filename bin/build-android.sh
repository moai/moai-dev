#!/bin/bash
if [ x$ANDROID_NDK == x ]; then
   echo ANDROID_NDK variable was not set. Please set to your ndk path
   exit 1
fi

which cmake || (echo Could not find cmake ; exit 1)

#get cores
if [ "$OSTYPE" == "linux-gnu" ]; then
  cores=$(getconf _NPROCESSORS_ONLN)
else
  cores=$(sysctl -n hw.logicalcpu_max)
fi

#override cores for ci
if [ "$CI" == "true" ]; then
  cores=2
fi

if [ x$1 == x ]; then
  libprefix=`dirname $0`/../lib/android
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




build_folder=$moai_root/build
#we must have a success from here on
set -e

ARCHS="armeabi armeabi-v7a x86"
for ARCH in $ARCHS
do

  cd $build_folder
  
  
  if ! [ -d "build-android-$ARCH" ]
  then
  mkdir build-android-$ARCH
  fi
  cd build-android-$ARCH

  cmake \
  -DBUILD_ANDROID=TRUE \
  -DCMAKE_TOOLCHAIN_FILE="$moai_root/cmake/hosts/host-android/android.toolchain.cmake" \
  -DCMAKE_BUILD_TYPE=Release \
  -DMOAI_LUAJIT=False \
  -DANDROID_ABI=$ARCH \
  -DCMAKE_INSTALL_PREFIX=$libprefix/$ARCH \
  -DLIBRARY_OUTPUT_PATH_ROOT=./build-android-$ARCH/ \
  $moai_root/cmake || exit 1

  cmake --build . --target install -- -j$cores
  echo Finished building $ARCH
done
