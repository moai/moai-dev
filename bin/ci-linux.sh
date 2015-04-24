#!/usr/bin/env bash

echo "Setting up MoaiUtil path..."

UTIL_PATH=$(dirname "${BASH_SOURCE[0]}")
UTIL_PATH=$(cd $UTIL_PATH/../util; pwd)
export PATH=$PATH:$UTIL_PATH

echo "getting latest cmake"
pushd ~
wget http://www.cmake.org/files/v3.1/cmake-3.1.3-Linux-x86_64.tar.gz
tar xvf cmake-3.1.3-Linux-x86_64.tar.gz
cd cmake-3.1.3-Linux-x86_64/bin
export PATH=$(pwd):$PATH
popd


pushd `dirname $0`
bash build-linux.sh
echo Linux Build Successful
popd





