#!/bin/bash

set -e # exit on error

pushd $(dirname "${0}") > /dev/null

pushd ../libmoai/ > /dev/null
./build-ios.sh
popd > /dev/null

xcodebuild -derivedDataPath build -configuration Release -project MoaiSample.xcodeproj -scheme moai -sdk iphonesimulator
xcodebuild -derivedDataPath build -configuration Release -project MoaiSample.xcodeproj -scheme moai -sdk iphoneos

popd > /dev/null