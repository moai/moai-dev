#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e # exit on error

pushd ../libmoai/ > /dev/null
./clean-ios.sh
popd > /dev/null

xcodebuild -derivedDataPath build -configuration Release -project MoaiSample.xcodeproj -scheme moai -sdk iphonesimulator clean
xcodebuild -derivedDataPath build -configuration Release -project MoaiSample.xcodeproj -scheme moai -sdk iphoneos clean
