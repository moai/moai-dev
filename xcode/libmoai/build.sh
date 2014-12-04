#!/bin/bash

xcodebuild -configuration Release -project libmoai.xcodeproj -target libmoai-osx-all
xcodebuild -configuration Release -project libmoai.xcodeproj -target libmoai-ios-all -sdk iphonesimulator
xcodebuild -configuration Release -project libmoai.xcodeproj -target libmoai-ios-all -sdk iphoneos

pushd ./build/Release-iphoneos/ > /dev/null
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

OSX_LIB=../../lib/osx

rm -rf $OSX_LIB
mkdir -p $OSX_LIB
cp -a ./build/Release/*.a $OSX_LIB

IOS_LIB=../../lib/ios

rm -rf $IOS_LIB
mkdir -p $IOS_LIB
cp -a ./build/Release-universal/*.a $IOS_LIB
