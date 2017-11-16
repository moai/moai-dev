#!/bin/bash

set -e # exit on error

./build-ios-simulator.sh $1 || exit 1
./build-ios-device.sh $1 || exit 1

pushd $(dirname "${0}")/../../xcode > /dev/null

IOS_LIB=../lib/ios

#---------------------------------------------------------------
# package libmoai ios

pushd ./build/Build/Products/Release-iphoneos/ > /dev/null
if [ -d "../Release-universal" ]; then
   rm -rf "../Release-universal" # clean out the old dir (if any)
fi
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

if [ -d  "$IOS_LIB" ]; then
  rm -rf $IOS_LIB
fi
mkdir -p $IOS_LIB
cp -a ./build/Build/Products/Release-universal/*.a $IOS_LIB

#---------------------------------------------------------------
# build ios static
if [ ! "$1"=="justlibs" ]; then
xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios-static -sdk iphonesimulator PLATFORM_NAME=iphonesimulator || exit 1
  xcodebuild -derivedDataPath build -configuration Release -workspace moai.xcworkspace -scheme moai-ios-static -sdk iphoneos || exit 1
fi

popd > /dev/null