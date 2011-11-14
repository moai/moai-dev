#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

#!/bin/bash

set -e

xcodebuild -configuration Release -target libmoai-ios-3rdparty -sdk iphonesimulator build
xcodebuild -configuration Release -target libmoai-ios -sdk iphonesimulator build
xcodebuild -configuration Release -target libmoai-ios-luaext -sdk iphonesimulator build
xcodebuild -configuration Release -target libmoai-ios-untz -sdk iphonesimulator build

xcodebuild -configuration Release -target libmoai-ios-3rdparty -sdk iphoneos build
xcodebuild -configuration Release -target libmoai-ios -sdk iphoneos build
xcodebuild -configuration Release -target libmoai-ios-luaext -sdk iphoneos build
xcodebuild -configuration Release -target libmoai-ios-untz -sdk iphoneos build

cd build
mkdir -p Release

lipo -create -output ./Release/libmoai-ios-3rdparty.a ./Release-iphoneos/libmoai-ios-3rdparty.a ./Release-iphonesimulator/libmoai-ios-3rdparty.a
lipo -create -output ./Release/libmoai-ios.a ./Release-iphoneos/libmoai-ios.a ./Release-iphonesimulator/libmoai-ios.a
lipo -create -output ./Release/libmoai-ios-luaext.a ./Release-iphoneos/libmoai-ios-luaext.a ./Release-iphonesimulator/libmoai-ios-luaext.a
lipo -create -output ./Release/libmoai-ios-untz.a ./Release-iphoneos/libmoai-ios-untz.a ./Release-iphonesimulator/libmoai-ios-untz.a

mkdir -p Release-armv6
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-3rdparty.a ./Release/libmoai-ios-3rdparty.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios.a ./Release/libmoai-ios.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-luaext.a ./Release/libmoai-ios-luaext.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-untz.a ./Release/libmoai-ios-untz.a

mkdir -p Release-armv7
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-3rdparty.a ./Release/libmoai-ios-3rdparty.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios.a ./Release/libmoai-ios.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-luaext.a ./Release/libmoai-ios-luaext.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-untz.a ./Release/libmoai-ios-untz.a

mkdir -p Release-i386
lipo -thin i386 -output ./Release-i386/libmoai-ios-3rdparty.a ./Release/libmoai-ios-3rdparty.a
lipo -thin i386 -output ./Release-i386/libmoai-ios.a ./Release/libmoai-ios.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-luaext.a ./Release/libmoai-ios-luaext.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-untz.a ./Release/libmoai-ios-untz.a


