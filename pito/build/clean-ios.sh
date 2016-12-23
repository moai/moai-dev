#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e # exit on error

./clean-ios-simulator.sh
./clean-ios-device.sh

pushd $(dirname "${0}")/../../xcode > /dev/null

IOS_LIB=../lib/ios

xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios-static -sdk iphonesimulator clean
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios-static -sdk iphoneos clean

rm -rf $IOS_LIB

popd > /dev/null