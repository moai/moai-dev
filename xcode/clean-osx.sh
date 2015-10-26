#!/bin/bash

#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

set -e # exit on error

OSX_LIB=../lib/osx
OSX_BIN=../bin/osx

pushd $(dirname "${0}") > /dev/null

#---------------------------------------------------------------
# clean everything

rm -rf $OSX_LIB
rm -rf $OSX_BIN

xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme libmoai-osx-all clean
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-osx clean
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-osx-static clean

popd > /dev/null