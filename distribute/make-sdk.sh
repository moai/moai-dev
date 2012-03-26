#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

# general files
mkdir -p moai-sdk/3rdparty/glut-3.7.6
cd ../3rdparty/glut-3.7.6-bin
cp -R * ../../distribute/moai-sdk/3rdparty/glut-3.7.6
cd ../../distribute

mkdir -p moai-sdk/samples
pushd ../samples > /dev/null
	find . -name ".?*" -type d -prune -o -name "test" -type d -prune -o -name "tutorials" -type d -prune -o -name "replace-run-bat-files.bat" -type f -prune -o -type f -print0 | cpio -pmd0 --quiet ../distribute/moai-sdk/samples
popd > /dev/null

mkdir -p moai-sdk/include/aku
cp -R ../src/aku/*.h moai-sdk/include/aku

mkdir -p moai-sdk/include/lua-headers
cp -R ../src/lua-headers/*.h moai-sdk/include/lua-headers

mkdir -p moai-sdk/include/lua-modules
cd ../src/lua-modules
cp -R * ../../distribute/moai-sdk/include/lua-modules
cd ../../distribute

# docs
mkdir moai-sdk/docs
if [ -d doxygen/html-lua/html ]; then
	cd doxygen/html-lua/html
	cp -R * ../../../moai-sdk/docs
	cd ../../..
else
	echo "*** Required documentation has not yet been generated."
	echo "*** Run ../doxygen/make-docs-lua.bat to generate the required "
	echo "*** documentation and then re-run this script to remake the SDK."
fi

mkdir -p moai-sdk/hosts/src
cd ../src/hosts
cp -R * ../../distribute/moai-sdk/hosts/src
cd ../../distribute

# android host
mkdir -p moai-sdk/hosts/ant
pushd ../ant > /dev/null
	./make-host.sh -p com.getmoai.samples -s
	pushd untitled-host > /dev/null	
		find . -name ".?*" -type d -prune -o -name "settings-local.sh" -type f -prune -o -type f -print0 | cpio -pmd0 --quiet ../../distribute/moai-sdk/hosts/ant
	popd > /dev/null
popd > /dev/null


# ios host
mkdir -p moai-sdk/hosts/xcode/ios/Classes
cd ../xcode/ios/Classes
cp -R * ../../../distribute/moai-sdk/hosts/xcode/ios/Classes
cd ../../../distribute

cp ../xcode/ios/Entitlements.plist moai-sdk/hosts/xcode/ios/Entitlements.plist
cp ../xcode/ios/Icon.png moai-sdk/hosts/xcode/ios/Icon.png
cp ../xcode/ios/Icon@2x.png moai-sdk/hosts/xcode/ios/Icon@2x.png
cp ../xcode/ios/Icon-72.png moai-sdk/hosts/xcode/ios/Icon-72.png
cp ../xcode/ios/Icon-Small.png moai-sdk/hosts/xcode/ios/Icon-Small.png
cp ../xcode/ios/Icon-Small@2x.png moai-sdk/hosts/xcode/ios/Icon-Small@2x.png
cp ../xcode/ios/Icon-Small-50.png moai-sdk/hosts/xcode/ios/Icon-Small-50.png
cp ../xcode/ios/Info.plist moai-sdk/hosts/xcode/ios/Info.plist
cp ../xcode/ios/MainWindow-iPad.xib moai-sdk/hosts/xcode/ios/MainWindow-iPad.xib
cp ../xcode/ios/MainWindow-iPhone.xib moai-sdk/hosts/xcode/ios/MainWindow-iPhone.xib
cp ../xcode/ios/main.mm moai-sdk/hosts/xcode/ios/main.mm
cp ../xcode/ios/package.sh moai-sdk/hosts/xcode/ios/package.sh
echo "../../../samples/anim/anim-basic" > moai-sdk/hosts/xcode/ios/mt.default
echo "../../../include/lua-modules lua-modules" >> moai-sdk/hosts/xcode/ios/mt.default

mkdir -p moai-sdk/hosts/xcode/ios/Libraries/AdColony
for file in `find ../3rdparty/adcolonyiOS-197/Library/ -name "*.a"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/AdColony ; done

mkdir -p moai-sdk/hosts/xcode/ios/Libraries/Crittercism
for file in `find ../3rdparty/crittercismiOS-2.9.37/CrittercismSDK/ -name "*.xib"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/Crittercism ; done
for file in `find ../3rdparty/crittercismiOS-2.9.37/CrittercismSDK/ -name "*.png"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/Crittercism ; done
for file in `find ../3rdparty/crittercismiOS-2.9.37/CrittercismSDK/ -name "*.a"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/Crittercism ; done

mkdir -p moai-sdk/hosts/xcode/ios/Libraries/Facebook
for file in `find ../3rdparty/facebookiOS/lib/facebook-ios-sdk/ -name "*.a"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/Facebook ; done
for file in `find ../3rdparty/facebookiOS/lib/facebook-ios-sdk/ -name "*.bundle"` ; do cp -R $file moai-sdk/hosts/xcode/ios/Libraries/Facebook ; done

mkdir -p moai-sdk/hosts/xcode/ios/Libraries/TapjoyConnect
for file in `find ../3rdparty/tapjoyiOS-8.1.5/TapjoyConnect/ -name "*.xib"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/TapjoyConnect ; done
for file in `find ../3rdparty/tapjoyiOS-8.1.5/TapjoyConnect/ -name "*.png"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/TapjoyConnect ; done
for file in `find ../3rdparty/tapjoyiOS-8.1.5/TapjoyConnect/ -name "*.sql"` ; do cp $file moai-sdk/hosts/xcode/ios/Libraries/TapjoyConnect ; done

cp ../version.txt moai-sdk/version.txt

# replace run scripts
cd moai-sdk-run-scripts
./replace-run-scripts.sh
cd ..