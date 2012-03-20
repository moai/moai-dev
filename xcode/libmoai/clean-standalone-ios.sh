#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios -sdk iphonesimulator clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-3rdparty -sdk iphonesimulator clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-3rdparty/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-adcolony -sdk iphonesimulator clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-adcolony/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-facebook -sdk iphonesimulator clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-facebook/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-luaext -sdk iphonesimulator clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-luaext/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-tapjoy -sdk iphonesimulator clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-tapjoy/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-untz -sdk iphonesimulator clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-untz/iphonesimulator/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-zipfs -sdk iphonesimulator clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-zipfs/iphonesimulator/Release

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios -sdk iphoneos clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-3rdparty -sdk iphoneos clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-3rdparty/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-adcolony -sdk iphoneos clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-adcolony/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-facebook -sdk iphoneos clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-facebook/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-luaext -sdk iphoneos clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-luaext/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-tapjoy -sdk iphoneos clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-tapjoy/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-untz -sdk iphoneos clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-untz/iphoneos/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-zipfs -sdk iphoneos clean CONFIGURATION_BUILD_DIR=/tmp/ios/beta/libmoai/libmoai-ios-zipfs/iphoneos/Release

rm -rf /tmp/ios/beta/libmoai/Release-universal/libmoai-ios*
rm -rf /tmp/ios/beta/libmoai/Release-armv6/libmoai-ios*
rm -rf /tmp/ios/beta/libmoai/Release-armv7/libmoai-ios*
rm -rf /tmp/ios/beta/libmoai/Release-i386/libmoai-ios*
