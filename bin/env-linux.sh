#!/bin/bash

if [ ! "$0" == "-bash" ]; then
  ruby -e "exit '$0'.end_with? '.sh'"
  if [ $? -ne 0 ]; then
  echo ERROR: This file is meant to be be sourced by running with: source ./bin/env-linux.sh
  echo was run with: $0
	exit 1
fi
fi

#--config
NDK_PATH=${NDK_PATH:-/Users/david/Downloads/android-ndk-r9d}
EMSDK_PATH=${EMSDK_PATH:-/Users/david/moai/emsdk_portable}
DOXYGEN_PATH=${DOXYGEN_PATH:-/Applications/Doxygen.app/Contents/Resources}


#--check for reqs
error=false
#emsdk wants python2
which python2 > /dev/null || { echo "python2 is not setup.Emscripten needs this, please use [ cd /usr/bin && sudo ln -s python2.7 python2 ]"; error=true; }

#emsdk wants java and so does android
which java > /dev/null || { echo "Java was not found"; error=true; }

#android sdk likes 1.7 of javac
javac -version 2>&1 | grep "javac 1\.[78]" > /dev/null || { echo "JDK was not at 1.7"; error=true; }


#--install
if [ "$error" == "false" ]; then
echo "Setting NDK path..."
export ANDROID_NDK=$NDK_PATH

echo "Setting Emscripten path..."
pushd . > /dev/null
cd $EMSDK_PATH
./emsdk construct_env
. ./emsdk_set_env.sh
popd > /dev/null

echo "Setting up MoaiUtil path..."

UTIL_PATH=$(dirname "${BASH_SOURCE[0]}")
UTIL_PATH=$(cd $UTIL_PATH/../util; pwd)
export PATH=$PATH:$UTIL_PATH:$DOXYGEN_PATH


echo "Path setup complete"
fi