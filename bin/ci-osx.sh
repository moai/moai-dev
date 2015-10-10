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

echo "installing xcpretty"
gem install xcpretty

#all success from here please
set -e

pushd `dirname $0`
bash build-osx.sh
echo OSX Lib Build Successful
popd

pushd `dirname $0`
bash build-ios.sh
echo IOS Lib Build Successful
popd

pushd `dirname $0`/..
sudo chmod a+x util/moai
sudo chmod a+x util/moaiutil
export MOAI_ROOT=$(pwd)
popd

pushd ~

echo Creating test project
mkdir testhost
cd testhost
cp -R $MOAI_ROOT/samples/hello-moai src/
moaiutil host init

echo Creating and building ios host
moaiutil host build ios

echo Creating and building osx host
moaiutil host build osx

popd




