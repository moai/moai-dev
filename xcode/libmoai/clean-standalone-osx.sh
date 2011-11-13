#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

#!/bin/bash

set -e

xcodebuild -configuration Release -target libmoai-osx-3rdparty -sdk macosx clean
xcodebuild -configuration Release -target libmoai-osx -sdk macosx clean
xcodebuild -configuration Release -target libmoai-osx-luaext -sdk macosx clean
xcodebuild -configuration Release -target libmoai-osx-untz -sdk macosx clean
