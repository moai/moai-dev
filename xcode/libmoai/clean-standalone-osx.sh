#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

#!/bin/bash

xcodebuild -configuration Release -target libmoai-osx-3rdparty -sdk macosx10.6 clean
xcodebuild -configuration Release -target libmoai-osx -sdk macosx10.6 clean
xcodebuild -configuration Release -target libmoai-osx-luaext -sdk macosx10.6 clean
xcodebuild -configuration Release -target libmoai-osx-untz -sdk macosx10.6 clean
