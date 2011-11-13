#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

#!/bin/bash

set -e

xcodebuild -configuration Release -target libmoai-ios-3rdparty -sdk iphonesimulator clean
xcodebuild -configuration Release -target libmoai-ios -sdk iphonesimulator clean
xcodebuild -configuration Release -target libmoai-ios-luaext -sdk iphonesimulator clean
xcodebuild -configuration Release -target libmoai-ios-untz -sdk iphonesimulator clean

xcodebuild -configuration Release -target libmoai-ios-3rdparty -sdk iphoneos clean
xcodebuild -configuration Release -target libmoai-ios -sdk iphoneos clean
xcodebuild -configuration Release -target libmoai-ios-luaext -sdk iphoneos clean
xcodebuild -configuration Release -target libmoai-ios-untz -sdk iphoneos clean
