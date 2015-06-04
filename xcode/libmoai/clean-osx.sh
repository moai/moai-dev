#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e # exit on error

xcodebuild -derivedDataPath build -configuration Release -project libmoai.xcodeproj -scheme libmoai-osx-all clean
