#!/bin/bash

set -e # exit on error

pushd ../libmoai/ > /dev/null
./build-ios.sh
popd > /dev/null

xcodebuild -derivedDataPath build -configuration Release -project MoaiSample.xcodeproj -scheme moai -sdk iphonesimulator
xcodebuild -derivedDataPath build -configuration Release -project MoaiSample.xcodeproj -scheme moai -sdk iphoneos
