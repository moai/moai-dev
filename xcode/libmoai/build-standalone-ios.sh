#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios -sdk iphonesimulator build build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-3rdparty -sdk iphonesimulator build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-luaext -sdk iphonesimulator build build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-tapjoy -sdk iphonesimulator build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-untz -sdk iphonesimulator build build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-zipfs -sdk iphonesimulator build OBJROOT=build SYMROOT=build

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios -sdk iphoneos build build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-3rdparty -sdk iphoneos build build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-luaext -sdk iphoneos build build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-tapjoy -sdk iphoneos build build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-untz -sdk iphoneos build build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-zipfs -sdk iphoneos build build OBJROOT=build SYMROOT=build

cd build

mkdir -p Release
lipo -create -output ./Release/libmoai-ios.a ./Release-iphoneos/libmoai-ios.a ./Release-iphonesimulator/libmoai-ios.a
lipo -create -output ./Release/libmoai-ios-3rdparty.a ./Release-iphoneos/libmoai-ios-3rdparty.a ./Release-iphonesimulator/libmoai-ios-3rdparty.a
lipo -create -output ./Release/libmoai-ios-luaext.a ./Release-iphoneos/libmoai-ios-luaext.a ./Release-iphonesimulator/libmoai-ios-luaext.a
lipo -create -output ./Release/libmoai-ios-tapjoy.a ./Release-iphoneos/libmoai-ios-tapjoy.a ./Release-iphonesimulator/libmoai-ios-tapjoy.a
lipo -create -output ./Release/libmoai-ios-untz.a ./Release-iphoneos/libmoai-ios-untz.a ./Release-iphonesimulator/libmoai-ios-untz.a
lipo -create -output ./Release/libmoai-ios-zipfs.a ./Release-iphoneos/libmoai-ios-zipfs.a ./Release-iphonesimulator/libmoai-ios-zipfs.a

mkdir -p Release-armv6
lipo -thin armv6 -output ./Release-armv6/libmoai-ios.a ./Release/libmoai-ios.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-3rdparty.a ./Release/libmoai-ios-3rdparty.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-luaext.a ./Release/libmoai-ios-luaext.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-tapjoy.a ./Release/libmoai-ios-tapjoy.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-untz.a ./Release/libmoai-ios-untz.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-zipfs.a ./Release/libmoai-ios-zipfs.a

mkdir -p Release-armv7
lipo -thin armv7 -output ./Release-armv7/libmoai-ios.a ./Release/libmoai-ios.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-3rdparty.a ./Release/libmoai-ios-3rdparty.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-luaext.a ./Release/libmoai-ios-luaext.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-tapjoy.a ./Release/libmoai-ios-tapjoy.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-untz.a ./Release/libmoai-ios-untz.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-zipfs.a ./Release/libmoai-ios-zipfs.a

mkdir -p Release-i386
lipo -thin i386 -output ./Release-i386/libmoai-ios.a ./Release/libmoai-ios.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-3rdparty.a ./Release/libmoai-ios-3rdparty.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-luaext.a ./Release/libmoai-ios-luaext.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-tapjoy.a ./Release/libmoai-ios-tapjoy.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-untz.a ./Release/libmoai-ios-untz.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-zipfs.a ./Release/libmoai-ios-zipfs.a


