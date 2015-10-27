#!/bin/bash

set -e # exit on error

pushd $(dirname "${0}")/../../xcode > /dev/null

OSX_LIB=../lib/osx
OSX_BIN=../bin/osx

#---------------------------------------------------------------
# build libmoai osx
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme libmoai-osx-all || exit 1

rm -rf $OSX_LIB
mkdir -p $OSX_LIB
cp -a ./build/Build/Products/Release/*.a $OSX_LIB

#---------------------------------------------------------------
# build osx
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-osx || exit 1

#---------------------------------------------------------------
# build osx static
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-osx-static || exit 1

popd > /dev/null