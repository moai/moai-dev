#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

./clean-sdk.sh

# general files
mkdir -p moai-sdk/3rdparty/glut-3.7.6

cd ../3rdparty/glut-3.7.6-bin
cp -R * ../../distribute/moai-sdk/3rdparty/glut-3.7.6
cd ../../distribute

mkdir -p moai-sdk/samples/basics
cd ../samples/basics
cp -R * ../../distribute/moai-sdk/samples/basics
cd ../../distribute

mkdir -p moai-sdk/samples/flash
cd ../samples/flash
cp -R * ../../distribute/moai-sdk/samples/flash
cd ../../distribute

mkdir -p moai-sdk/samples/config
cd ../samples/config
cp -R * ../../distribute/moai-sdk/samples/config
cd ../../distribute

mkdir -p moai-sdk/samples/iphone
cd ../samples/iphone
cp -R * ../../distribute/moai-sdk/samples/iphone
cd ../../distribute

mkdir -p moai-sdk/include/aku
cp -R ../src/aku/*.h moai-sdk/include/aku

mkdir -p moai-sdk/hosts/src

cp ../version.txt moai-sdk/version.txt

cd ../src/hosts
cp -R * ../../distribute/moai-sdk/hosts/src
cd ../../distribute

# docs
mkdir moai-sdk/docs
cd doxygen/html-lua/html
cp -R * ../../../moai-sdk/docs
cd ../../..

# android host
mkdir -p moai-sdk/hosts/eclipse/android-project

cd ../eclipse/android-project
cp -R . ../../distribute/moai-sdk/hosts/eclipse/android-project
cd ../../distribute

cp -f moai-sdk/hosts/eclipse/packager/moai-target.default moai-sdk/hosts/eclipse/android-project/moai-target
cp -f moai-sdk/hosts/eclipse/packager/default.project moai-sdk/hosts/eclipse/android-project/.project

# ios host
mkdir -p moai-sdk/hosts/xcode-ios/Classes

cd ../xcode/ios/Classes
cp -R * ../../../distribute/moai-sdk/hosts/xcode-ios/Classes
cd ../../../distribute

cp ../xcode/ios/Entitlements.plist moai-sdk/hosts/xcode-ios/Entitlements.plist
cp ../xcode/ios/Icon.png moai-sdk/hosts/xcode-ios/Icon.png
cp ../xcode/ios/Icon@2x.png moai-sdk/hosts/xcode-ios/Icon@2x.png
cp ../xcode/ios/Icon-72.png moai-sdk/hosts/xcode-ios/Icon-72.png
cp ../xcode/ios/Icon-Small.png moai-sdk/hosts/xcode-ios/Icon-Small.png
cp ../xcode/ios/Icon-Small@2x.png moai-sdk/hosts/xcode-ios/Icon-Small@2x.png
cp ../xcode/ios/Icon-Small-50.png moai-sdk/hosts/xcode-ios/Icon-Small-50.png
cp ../xcode/ios/Info.plist moai-sdk/hosts/xcode-ios/Info.plist
cp ../xcode/ios/MainWindow-iPad.xib moai-sdk/hosts/xcode-ios/MainWindow-iPad.xib
cp ../xcode/ios/MainWindow-iPhone.xib moai-sdk/hosts/xcode-ios/MainWindow-iPhone.xib
cp ../xcode/ios/main.mm moai-sdk/hosts/xcode-ios/main.mm
cp ../xcode/ios/mt.default moai-sdk/hosts/xcode-ios/mt.default
cp ../xcode/ios/package.sh moai-sdk/hosts/xcode-ios/package.sh

# replace run scripts
cd moai-sdk-run-scripts
./replace-run-scripts.sh
cd ..

