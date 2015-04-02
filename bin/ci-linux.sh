#!/usr/bin/env bash

echo "Setting up MoaiUtil path..."

UTIL_PATH=$(dirname "${BASH_SOURCE[0]}")
UTIL_PATH=$(cd $UTIL_PATH/../util; pwd)
export PATH=$PATH:$UTIL_PATH

echo "Checking for cmake"
cmake --version 2>&1 | grep "cmake [32]\.[18]" > /dev/null || { echo "cmake was not at 2.8+"; exit 1 }

if [ "$BUILD_TYPE" == "linux" ]; then
  cd `dirname $0`
  ./build-linux.sh
fi

if [ "$BUILD_TYPE" == "android" ]; then
  echo "Android Build on Linux not configured yet"
fi
