#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

#!/bin/bash

set -e

xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-3rdparty -sdk macosx build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx -sdk macosx build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-luaext -sdk macosx build OBJROOT=build SYMROOT=build
xcodebuild -configuration Release -workspace libmoai.xcodeproj/project.xcworkspace -scheme libmoai-osx-untz -sdk macosx build OBJROOT=build SYMROOT=build
