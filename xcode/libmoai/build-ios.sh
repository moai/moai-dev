#!/bin/bash

set -e # exit on error

xcodebuild -derivedDataPath build -configuration Release -project libmoai.xcodeproj -scheme libmoai-ios-all -sdk iphonesimulator
xcodebuild -derivedDataPath build -configuration Release -project libmoai.xcodeproj -scheme libmoai-ios-all -sdk iphoneos

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

IOS_LIB=../../lib/ios

rm -rf $IOS_LIB
mkdir -p $IOS_LIB
cp -a ./build/Build/Products/Release-universal/*.a $IOS_LIB
