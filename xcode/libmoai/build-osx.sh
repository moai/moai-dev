#!/bin/bash

set -e # exit on error

pushd $(dirname "${0}") > /dev/null

xcodebuild -derivedDataPath build -configuration Release -project libmoai.xcodeproj -scheme libmoai-osx-all

OSX_LIB=../../lib/osx

rm -rf $OSX_LIB
mkdir -p $OSX_LIB
cp -a ./build/Build/Products/Release/*.a $OSX_LIB

popd > /dev/null