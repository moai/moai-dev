#!/bin/bash

xcodebuild -configuration Release -project MoaiSample.xcodeproj -target moai || exit 1

OSX_BIN=../../bin/osx

rm -rf $OSX_BIN
mkdir -p $OSX_BIN
cp -a ./build/Release/moai $OSX_BIN
