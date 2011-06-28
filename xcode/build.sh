#----------------------------------------------------------------#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#----------------------------------------------------------------#

#!/bin/bash

cd libmoai

xcodebuild -nodistribute -configuration Release -target libmoai-iphone-3rdparty -sdk iphonesimulator4.3 build
xcodebuild -nodistribute -configuration Release -target libmoai-iphone -sdk iphonesimulator4.3 build
xcodebuild -nodistribute -configuration Release -target libmoai-iphone-luaext -sdk iphonesimulator4.3 build
xcodebuild -nodistribute -configuration Release -target libmoai-iphone-untz -sdk iphonesimulator4.3 build

xcodebuild -nodistribute -configuration Release -target libmoai-iphone-3rdparty -sdk iphoneos4.3 build
xcodebuild -nodistribute -configuration Release -target libmoai-iphone -sdk iphoneos4.3 build
xcodebuild -nodistribute -configuration Release -target libmoai-iphone-luaext -sdk iphoneos4.3 build
xcodebuild -nodistribute -configuration Release -target libmoai-iphone-untz -sdk iphoneos4.3 build

xcodebuild -nodistribute -configuration Release -target libmoai-osx-3rdparty -sdk macosx10.6 build
xcodebuild -nodistribute -configuration Release -target libmoai-osx -sdk macosx10.6 build
xcodebuild -nodistribute -configuration Release -target libmoai-osx-luaext -sdk macosx10.6 build
xcodebuild -nodistribute -configuration Release -target libmoai-osx-untz -sdk macosx10.6 build

cd build

lipo -create -output ./Release/libmoai-iphone-3rdparty.a ./Release-iphoneos/libmoai-iphone-3rdparty.a ./Release-iphonesimulator/libmoai-iphone-3rdparty.a
lipo -create -output ./Release/libmoai-iphone.a ./Release-iphoneos/libmoai-iphone.a ./Release-iphonesimulator/libmoai-iphone.a
lipo -create -output ./Release/libmoai-iphone-luaext.a ./Release-iphoneos/libmoai-iphone-luaext.a ./Release-iphonesimulator/libmoai-iphone-luaext.a
lipo -create -output ./Release/libmoai-iphone-untz.a ./Release-iphoneos/libmoai-iphone-untz.a ./Release-iphonesimulator/libmoai-iphone-untz.a

