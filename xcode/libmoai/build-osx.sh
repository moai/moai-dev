#!/bin/bash

set -e # exit on error

xcodebuild -derivedDataPath build -configuration Release -project libmoai.xcodeproj -scheme libmoai-osx-all

OSX_LIB=../../lib/osx

rm -rf $OSX_LIB
mkdir -p $OSX_LIB
cp -a ./build/Build/Products/Release/*.a $OSX_LIB
