#!/bin/bash

xcodebuild -configuration Release -project libmoai.xcodeproj -target libmoai-osx-all
xcodebuild -configuration Release -project libmoai.xcodeproj -target libmoai-ios-all -sdk iphonesimulator
xcodebuild -configuration Release -project libmoai.xcodeproj -target libmoai-ios-all -sdk iphoneos

pushd ./build/Release-iphoneos/ > /dev/null
rm -rf "../universal" # clean out the old dir (if any)
mkdir -p "../universal" # make the new dir
for f in *.a
do
	echo $f
	libs="$f ../Release-iphonesimulator/$f"

	if ! xcrun lipo -create -output "../universal/$f" $libs; then
		echo >&2 "lipo failed, giving up."
		exit 1
	fi
done
popd > /dev/null

OSX_BIN=../../bin/osx

rm -rf $OSX_BIN
mkdir -p $OSX_BIN
cp -a ./build/Release/*.a $OSX_BIN

IOS_BIN=../../bin/ios

rm -rf $IOS_BIN
mkdir -p $IOS_BIN
cp -a ./build/universal/*.a $IOS_BIN
