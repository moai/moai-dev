#!/usr/bin/env bash

echo "Setting up MoaiUtil path..."

UTIL_PATH=$(dirname "${BASH_SOURCE[0]}")
UTIL_PATH=$(cd $UTIL_PATH/../util; pwd)
export PATH=$PATH:$UTIL_PATH

echo "getting latest cmake"
pushd ~
wget http://www.cmake.org/files/v3.1/cmake-3.1.3-Darwin-x86_64.tar.gz
tar xvf cmake-3.1.3-Darwin-x86_64.tar.gz
cd cmake-3.1.3-Darwin-x86_64/bin
export PATH=$(pwd):$PATH
popd


pushd `dirname $0`
bash build-osx.sh
echo OSX Lib Build Successful
popd

pushd `dirname $0`
bash build-ios.sh
echo IOS Lib Build Successful
popd

pushd `dirname $0`/..
echo Creating and building android host
sudo chmod a+x util/moai
sudo chmod a+x util/moaiutil
export MOAI_ROOT=$(pwd)
popd

pushd ~
mkdir testhost
cd testhost
cp -R $MOAI_ROOT/samples/hello-moai src/
moaiutil host init
moaiutil host build ios
popd




