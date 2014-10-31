#!/bin/bash



xcodebuild -configuration Release -project libmoai.xcodeproj -target libmoai-osx-all

xcodebuild -configuration Release -project libmoai.xcodeproj -target libmoai-ios-all -sdk iphonesimulator

xcodebuild -configuration Release -project libmoai.xcodeproj -target libmoai-ios-all -sdk iphoneos

cd ./build/Release-iphoneos/
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

