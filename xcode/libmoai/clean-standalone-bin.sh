#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

#!/bin/bash

xcodebuild -nodistribute -configuration Release -target libmoai-iphone-3rdparty -sdk iphonesimulator4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-iphone -sdk iphonesimulator4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-iphone-luaext -sdk iphonesimulator4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-iphone-untz -sdk iphonesimulator4.3 clean

xcodebuild -nodistribute -configuration Release -target libmoai-iphone-3rdparty -sdk iphoneos4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-iphone -sdk iphoneos4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-iphone-luaext -sdk iphoneos4.3 clean
xcodebuild -nodistribute -configuration Release -target libmoai-iphone-untz -sdk iphoneos4.3 clean

xcodebuild -nodistribute -configuration Release -target libmoai-osx-3rdparty -sdk macosx10.6 clean
xcodebuild -nodistribute -configuration Release -target libmoai-osx -sdk macosx10.6 clean
xcodebuild -nodistribute -configuration Release -target libmoai-osx-luaext -sdk macosx10.6 clean
xcodebuild -nodistribute -configuration Release -target libmoai-osx-untz -sdk macosx10.6 clean
