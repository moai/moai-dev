#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

#!/bin/bash

xcodebuild -nodistribute -configuration Release -target libmoai-ios-3rdparty -sdk iphonesimulator4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-ios -sdk iphonesimulator4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-ios-luaext -sdk iphonesimulator4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-ios-untz -sdk iphonesimulator4.3 clean

xcodebuild -nodistribute -configuration Release -target libmoai-ios-3rdparty -sdk iphoneos4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-ios -sdk iphoneos4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-ios-luaext -sdk iphoneos4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-ios-untz -sdk iphoneos4.3 clean

xcodebuild -nodistribute -configuration Release -target libmoai-osx-3rdparty -sdk macosx10.6 clean
xcodebuild -nodistribute -configuration Release -target libmoai-osx -sdk macosx10.6 clean
xcodebuild -nodistribute -configuration Release -target libmoai-osx-luaext -sdk macosx10.6 clean
xcodebuild -nodistribute -configuration Release -target libmoai-osx-untz -sdk macosx10.6 clean
