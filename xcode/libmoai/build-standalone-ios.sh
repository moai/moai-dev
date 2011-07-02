#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

#!/bin/bash

xcodebuild -configuration Release -target libmoai-ios-3rdparty -sdk iphonesimulator4.3 build
xcodebuild -configuration Release -target libmoai-ios -sdk iphonesimulator4.3 build
xcodebuild -configuration Release -target libmoai-ios-luaext -sdk iphonesimulator4.3 build
xcodebuild -configuration Release -target libmoai-ios-untz -sdk iphonesimulator4.3 build

xcodebuild -configuration Release -target libmoai-ios-3rdparty -sdk iphoneos4.3 build
xcodebuild -configuration Release -target libmoai-ios -sdk iphoneos4.3 build
xcodebuild -configuration Release -target libmoai-ios-luaext -sdk iphoneos4.3 build
xcodebuild -configuration Release -target libmoai-ios-untz -sdk iphoneos4.3 build

cd build
mkdir -p Release

lipo -create -output ./Release/libmoai-ios-3rdparty.a ./Release-iphoneos/libmoai-ios-3rdparty.a ./Release-iphonesimulator/libmoai-ios-3rdparty.a
lipo -create -output ./Release/libmoai-ios.a ./Release-iphoneos/libmoai-ios.a ./Release-iphonesimulator/libmoai-ios.a
lipo -create -output ./Release/libmoai-ios-luaext.a ./Release-iphoneos/libmoai-ios-luaext.a ./Release-iphonesimulator/libmoai-ios-luaext.a
lipo -create -output ./Release/libmoai-ios-untz.a ./Release-iphoneos/libmoai-ios-untz.a ./Release-iphonesimulator/libmoai-ios-untz.a

