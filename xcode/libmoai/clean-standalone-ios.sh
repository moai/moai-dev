#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

#!/bin/bash

set -e

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-3rdparty -sdk iphonesimulator clean OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios -sdk iphonesimulator clean OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-luaext -sdk iphonesimulator clean OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-untz -sdk iphonesimulator clean OBJROOT=build SYMROOT=build

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-3rdparty -sdk iphoneos clean OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios -sdk iphoneos clean OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-luaext -sdk iphoneos clean OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-ios-untz -sdk iphoneos clean OBJROOT=build SYMROOT=build

rm -rf build/Release
rm -rf build/Release-armv7
rm -rf build/Release-i386
