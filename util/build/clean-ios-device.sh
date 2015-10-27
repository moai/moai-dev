#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e # exit on error

pushd $(dirname "${0}")/../../xcode > /dev/null

xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme libmoai-ios-all -sdk iphoneos clean
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios -sdk iphoneos clean

popd > /dev/null