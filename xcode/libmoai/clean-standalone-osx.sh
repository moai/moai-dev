#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-3rdparty -sdk macosx clean OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx -sdk macosx clean OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-luaext -sdk macosx clean OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-untz -sdk macosx clean OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-zipfs -sdk macosx clean OBJROOT=build SYMROOT=build
