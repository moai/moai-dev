#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx -sdk macosx build CONFIGURATION_BUILD_DIR=/tmp/osx/beta/libmoai/libmoai-osx/macosx/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-3rdparty -sdk macosx build CONFIGURATION_BUILD_DIR=/tmp/osx/beta/libmoai/libmoai-osx-3rdparty/macosx/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-luaext -sdk macosx build CONFIGURATION_BUILD_DIR=/tmp/osx/beta/libmoai/libmoai-osx-luaext/macosx/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-untz -sdk macosx build CONFIGURATION_BUILD_DIR=/tmp/osx/beta/libmoai/libmoai-osx-untz/macosx/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-zipfs -sdk macosx build CONFIGURATION_BUILD_DIR=/tmp/osx/beta/libmoai/libmoai-osx-zipfs/macosx/Release

cd /tmp/osx/beta/libmoai

mkdir -p Release
cp ./libmoai-osx/macosx/Release/libmoai-osx.a ./Release/libmoai-osx.a
cp ./libmoai-osx-3rdparty/macosx/Release/libmoai-osx-3rdparty.a ./Release/libmoai-osx-3rdparty.a
cp ./libmoai-osx-luaext/macosx/Release/libmoai-osx-luaext.a ./Release/libmoai-osx-luaext.a
cp ./libmoai-osx-untz/macosx/Release/libmoai-osx-untz.a ./Release/libmoai-osx-untz.a
cp ./libmoai-osx-zipfs/macosx/Release/libmoai-osx-zipfs.a ./Release/libmoai-osx-zipfs.a

cd -
