#!/bin/bash

set -e # exit on error

IOS_LIB=../lib/ios

pushd $(dirname "${0}") > /dev/null

#---------------------------------------------------------------
# build libmoai ios
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme libmoai-ios-all -sdk iphonesimulator || exit 1
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme libmoai-ios-all -sdk iphoneos || exit 1

pushd ./build/Build/Products/Release-iphoneos/ > /dev/null
rm -rf "../Release-universal" # clean out the old dir (if any)
mkdir -p "../Release-universal" # make the new dir
for f in *.a
do
	echo $f
	libs="$f ../Release-iphonesimulator/$f"

	if ! xcrun lipo -create -output "../Release-universal/$f" $libs; then
		echo >&2 "lipo failed, giving up."
		exit 1
	fi
done
popd > /dev/null

rm -rf $IOS_LIB
mkdir -p $IOS_LIB
cp -a ./build/Build/Products/Release-universal/*.a $IOS_LIB

#---------------------------------------------------------------
# build ios
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios -sdk iphonesimulator || exit 1
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios -sdk iphoneos || exit 1

#---------------------------------------------------------------
# build ios static
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios-static -sdk iphonesimulator || exit 1
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios-static -sdk iphoneos || exit 1


popd > /dev/null