#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e # exit on error

IOS_LIB=../lib/ios

pushd $(dirname "${0}") > /dev/null

#---------------------------------------------------------------
# clean everything

rm -rf $IOS_LIB

xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme libmoai-ios-all -sdk iphonesimulator clean
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme libmoai-ios-all -sdk iphoneos clean

xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios -sdk iphonesimulator clean
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios -sdk iphoneos clean

xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios-static -sdk iphonesimulator clean
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios-static -sdk iphoneos clean

popd > /dev/null