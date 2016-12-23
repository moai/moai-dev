#!/bin/bash

set -e # exit on error

pushd "$1" > /dev/null

pushd xcode/ios/ > /dev/null
xcodebuild -derivedDataPath build -configuration Debug -project MoaiSample.xcodeproj -scheme moai
xcodebuild -derivedDataPath build -configuration Release -project MoaiSample.xcodeproj -scheme moai
rm -rf ./build
popd > /dev/null

pushd xcode/osx/ > /dev/null
xcodebuild -derivedDataPath build -configuration Debug -project Moai.xcodeproj -scheme moai
xcodebuild -derivedDataPath build -configuration Release -project Moai.xcodeproj -scheme moai
rm -rf ./build
popd > /dev/null

popd > /dev/null
