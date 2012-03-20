#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios -sdk iphonesimulator build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-3rdparty -sdk iphonesimulator build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-3rdparty/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-adcolony -sdk iphonesimulator build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-adcolony/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-facebook -sdk iphonesimulator build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-facebook/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-luaext -sdk iphonesimulator build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-luaext/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-tapjoy -sdk iphonesimulator build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-tapjoy/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-untz -sdk iphonesimulator build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-untz/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-zipfs -sdk iphonesimulator build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-zipfs/iphonesimulator/Release

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios -sdk iphoneos build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-3rdparty -sdk iphoneos build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-3rdparty/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-adcolony -sdk iphoneos build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-adcolony/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-facebook -sdk iphoneos build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-facebook/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-luaext -sdk iphoneos build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-luaext/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-tapjoy -sdk iphoneos build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-tapjoy/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-untz -sdk iphoneos build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-untz/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-zipfs -sdk iphoneos build CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-zipfs/iphoneos/Release

cd /tmp/ios/beta/libmoai

mkdir -p Release-universal
lipo -create -output ./Release-universal/libmoai-ios.a ./libmoai-ios/iphonesimulator/Release/libmoai-ios.a ./libmoai-ios/iphoneos/Release/libmoai-ios.a
lipo -create -output ./Release-universal/libmoai-ios-3rdparty.a ./libmoai-ios-3rdparty/iphonesimulator/Release/libmoai-ios-3rdparty.a ./libmoai-ios-3rdparty/iphoneos/Release/libmoai-ios-3rdparty.a
lipo -create -output ./Release-universal/libmoai-ios-adcolony.a ./libmoai-ios-adcolony/iphonesimulator/Release/libmoai-ios-adcolony.a ./libmoai-ios-adcolony/iphoneos/Release/libmoai-ios-adcolony.a
lipo -create -output ./Release-universal/libmoai-ios-facebook.a ./libmoai-ios-facebook/iphonesimulator/Release/libmoai-ios-facebook.a ./libmoai-ios-facebook/iphoneos/Release/libmoai-ios-facebook.a
lipo -create -output ./Release-universal/libmoai-ios-luaext.a ./libmoai-ios-luaext/iphonesimulator/Release/libmoai-ios-luaext.a ./libmoai-ios-luaext/iphoneos/Release/libmoai-ios-luaext.a
lipo -create -output ./Release-universal/libmoai-ios-tapjoy.a ./libmoai-ios-tapjoy/iphonesimulator/Release/libmoai-ios-tapjoy.a ./libmoai-ios-tapjoy/iphoneos/Release/libmoai-ios-tapjoy.a
lipo -create -output ./Release-universal/libmoai-ios-untz.a ./libmoai-ios-untz/iphonesimulator/Release/libmoai-ios-untz.a ./libmoai-ios-untz/iphoneos/Release/libmoai-ios-untz.a
lipo -create -output ./Release-universal/libmoai-ios-zipfs.a ./libmoai-ios-zipfs/iphonesimulator/Release/libmoai-ios-zipfs.a ./libmoai-ios-zipfs/iphoneos/Release/libmoai-ios-zipfs.a

mkdir -p Release-armv6
lipo -thin armv6 -output ./Release-armv6/libmoai-ios.a ./Release-universal/libmoai-ios.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-3rdparty.a ./Release-universal/libmoai-ios-3rdparty.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-adcolony.a ./Release-universal/libmoai-ios-adcolony.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-facebook.a ./Release-universal/libmoai-ios-facebook.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-luaext.a ./Release-universal/libmoai-ios-luaext.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-tapjoy.a ./Release-universal/libmoai-ios-tapjoy.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-untz.a ./Release-universal/libmoai-ios-untz.a
lipo -thin armv6 -output ./Release-armv6/libmoai-ios-zipfs.a ./Release-universal/libmoai-ios-zipfs.a

mkdir -p Release-armv7
lipo -thin armv7 -output ./Release-armv7/libmoai-ios.a ./Release-universal/libmoai-ios.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-3rdparty.a ./Release-universal/libmoai-ios-3rdparty.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-adcolony.a ./Release-universal/libmoai-ios-adcolony.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-facebook.a ./Release-universal/libmoai-ios-facebook.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-luaext.a ./Release-universal/libmoai-ios-luaext.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-tapjoy.a ./Release-universal/libmoai-ios-tapjoy.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-untz.a ./Release-universal/libmoai-ios-untz.a
lipo -thin armv7 -output ./Release-armv7/libmoai-ios-zipfs.a ./Release-universal/libmoai-ios-zipfs.a

mkdir -p Release-i386
lipo -thin i386 -output ./Release-i386/libmoai-ios.a ./Release-universal/libmoai-ios.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-3rdparty.a ./Release-universal/libmoai-ios-3rdparty.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-adcolony.a ./Release-universal/libmoai-ios-adcolony.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-facebook.a ./Release-universal/libmoai-ios-facebook.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-luaext.a ./Release-universal/libmoai-ios-luaext.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-tapjoy.a ./Release-universal/libmoai-ios-tapjoy.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-untz.a ./Release-universal/libmoai-ios-untz.a
lipo -thin i386 -output ./Release-i386/libmoai-ios-zipfs.a ./Release-universal/libmoai-ios-zipfs.a

cd -
