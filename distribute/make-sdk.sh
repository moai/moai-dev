#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

set -e

mkdir -p moai-sdk/3rdparty/glut-3.7.6
pushd ../3rdparty/glut-3.7.6-bin > /dev/null
	cp -R * ../../distribute/moai-sdk/3rdparty/glut-3.7.6
popd > /dev/null

mkdir -p moai-sdk/samples
pushd ../samples > /dev/null
	find . -name ".?*" -type d -prune -o -name "test" -type d -prune -o -name "tutorials" -type d -prune -o -name "replace-run-bat-files.bat" -type f -prune -o -type f -print0 | cpio -pmd0 --quiet ../distribute/moai-sdk/samples
popd > /dev/null

mkdir -p moai-sdk/include/aku
pushd ../src/aku > /dev/null
	cp -R *.h ../../distribute/moai-sdk/include/aku
popd > /dev/null

mkdir -p moai-sdk/include/lua-headers
pushd ../src/lua-headers > /dev/null
	cp -R *.h ../../distribute/moai-sdk/include/lua-headers
popd > /dev/null

mkdir -p moai-sdk/include/lua-modules
pushd ../src/lua-modules > /dev/null
	cp -R * ../../distribute/moai-sdk/include/lua-modules
popd > /dev/null

mkdir -p moai-sdk/docs
if [ -d doxygen/html-lua/html ]; then
	pushd doxygen/html-lua/html > /dev/null
		cp -R * ../../../moai-sdk/docs
	popd > /dev/null
else
	echo "*** Required documentation has not been generated. Create the"
	echo "*** required documentation by running doxygen/make-docs-lua.bat"
	echo "*** and then re-run this script to generate the binary SDK."
fi

mkdir -p moai-sdk/hosts/src
pushd ../src/hosts > /dev/null
	cp -R * ../../distribute/moai-sdk/hosts/src
popd > /dev/null

mkdir -p moai-sdk/hosts/ant
pushd ../ant > /dev/null
	./make-host.sh -p com.getmoai.samples -s
	pushd untitled-host > /dev/null	
		find . -name ".?*" -type d -prune -o -name "settings-local.sh" -type f -prune -o -type f -print0 | cpio -pmd0 --quiet ../../distribute/moai-sdk/hosts/ant
	popd > /dev/null
popd > /dev/null

mkdir -p moai-sdk/hosts/xcode/ios/Classes
pushd ../xcode/ios > /dev/null
	cp Classes/* ../../distribute/moai-sdk/hosts/xcode/ios/Classes
	cp Entitlements.plist ../../distribute/moai-sdk/hosts/xcode/ios/Entitlements.plist
	cp Icon.png ../../distribute/moai-sdk/hosts/xcode/ios/Icon.png
	cp Icon@2x.png ../../distribute/moai-sdk/hosts/xcode/ios/Icon@2x.png
	cp Icon-72.png ../../distribute/moai-sdk/hosts/xcode/ios/Icon-72.png
	cp Icon-Small.png ../../distribute/moai-sdk/hosts/xcode/ios/Icon-Small.png
	cp Icon-Small@2x.png ../../distribute/moai-sdk/hosts/xcode/ios/Icon-Small@2x.png
	cp Icon-Small-50.png ../../distribute/moai-sdk/hosts/xcode/ios/Icon-Small-50.png
	cp Info.plist ../../distribute/moai-sdk/hosts/xcode/ios/Info.plist
	cp main.mm ../../distribute/moai-sdk/hosts/xcode/ios/main.mm
	cp package.sh ../../distribute/moai-sdk/hosts/xcode/ios/package.sh
popd > /dev/null

echo "../../../samples/anim/anim-basic" > moai-sdk/hosts/xcode/ios/mt.default
echo "../../../include/lua-modules lua-modules" >> moai-sdk/hosts/xcode/ios/mt.default

mkdir -p moai-sdk/hosts/xcode/ios/Libraries/Crittercism
for file in `find ../3rdparty/crittercismiOS-3.3.3/CrittercismSDK-crashonly/ -name "*.xib"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/Crittercism ; done
for file in `find ../3rdparty/crittercismiOS-3.3.3/CrittercismSDK-crashonly/ -name "*.png"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/Crittercism ; done
for file in `find ../3rdparty/crittercismiOS-3.3.3/CrittercismSDK-crashonly/ -name "*.a"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/Crittercism ; done

mkdir -p moai-sdk/hosts/xcode/ios/Libraries/Facebook
for file in `find ../3rdparty/facebookiOS-3.0.6.b/ -name "*.a"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/Facebook ; done
for file in `find ../3rdparty/facebookiOS-3.0.6.b/ -name "*.bundle"` ; do cp -R $file moai-sdk/hosts/xcode/ios/Libraries/Facebook ; done

mkdir -p moai-sdk/hosts/xcode/ios/Libraries/TapjoyConnect
for file in `find ../3rdparty/tapjoyiOS-8.1.9/TapjoyConnect/ -name "*.xib"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/TapjoyConnect ; done
for file in `find ../3rdparty/tapjoyiOS-8.1.9/TapjoyConnect/ -name "*.png"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/TapjoyConnect ; done
for file in `find ../3rdparty/tapjoyiOS-8.1.9/TapjoyConnect/ -name "*.sql"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/TapjoyConnect ; done

cp ../version.txt moai-sdk/version.txt

pushd moai-sdk-run-scripts > /dev/null
	./replace-run-scripts.sh
popd > /dev/null