#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx -sdk macosx clean CONFIGURATION_BUILD_DIR=/tmp/osx/beta/libmoai/libmoai-osx/macosx/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-3rdparty -sdk macosx clean CONFIGURATION_BUILD_DIR=/tmp/osx/beta/libmoai/libmoai-osx/macosx/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-luaext -sdk macosx clean CONFIGURATION_BUILD_DIR=/tmp/osx/beta/libmoai/libmoai-osx/macosx/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-untz -sdk macosx clean CONFIGURATION_BUILD_DIR=/tmp/osx/beta/libmoai/libmoai-osx/macosx/Release
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-zipfs -sdk macosx clean CONFIGURATION_BUILD_DIR=/tmp/osx/beta/libmoai/libmoai-osx/macosx/Release

rm -rf /tmp/osx/beta/libmoai/Release/libmoai-osx*