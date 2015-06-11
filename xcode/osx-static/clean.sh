#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e # exit on error

pushd ../libmoai/ > /dev/null
./clean-osx.sh
popd > /dev/null

xcodebuild -derivedDataPath build -configuration Release -project Moai.xcodeproj -scheme moai clean
