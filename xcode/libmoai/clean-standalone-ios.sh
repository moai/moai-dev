#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

#!/bin/bash

xcodebuild -configuration Release -target libmoai-ios-3rdparty -sdk iphonesimulator4.3 clean
xcodebuild configuration Release -target libmoai-ios -sdk iphonesimulator4.3 clean
xcodebuild -configuration Release -target libmoai-ios-luaext -sdk iphonesimulator4.3 clean
xcodebuild -configuration Release -target libmoai-ios-untz -sdk iphonesimulator4.3 clean

xcodebuild -configuration Release -target libmoai-ios-3rdparty -sdk iphoneos4.3 clean
xcodebuild -configuration Release -target libmoai-ios -sdk iphoneos4.3 clean
xcodebuild -configuration Release -target libmoai-ios-luaext -sdk iphoneos4.3 clean
xcodebuild -configuration Release -target libmoai-ios-untz -sdk iphoneos4.3 clean
