#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

./clean-sdk.sh

# general files
mkdir -p sdk/3rdparty/glut-3.7.6
cp -R ../3rdparty/glut-3.7.6-bin sdk/3rdparty/glut-3.7.6

mkdir -p sdk/samples
cp -R ../samples/basics sdk/samples

mkdir -p sdk/samples/hello-moai
cp -R ../samples/hello-moai sdk/samples/hello-moai

mkdir -p sdk/samples/config
cp -R ../samples/config sdk/samples/config

mkdir -p sdk/include/aku
cp -R ../src/aku/*.h sdk/include/aku

mkdir -p sdk/hosts/src
cp -R ../src/hosts sdk/hosts/src

# android host
mkdir -p sdk/hosts/eclipse
cp -R ../eclipse sdk/hosts/eclipse/android-project

# ios host
mkdir -p sdk/hosts/xcode-ios/Classes
cp -R ../xcode/ios/Classes sdk/hosts/xcode-ios/Classes

cp ../xcode/ios/Entitlements.plist sdk/hosts/xcode-ios/Entitlements.plist
cp ../xcode/ios/Icon.png sdk/hosts/xcode-ios/Icon.png
cp ../xcode/ios/Icon@2x.png sdk/hosts/xcode-ios/Icon@2x.png
cp ../xcode/ios/Icon-72.png sdk/hosts/xcode-ios/Icon-72.png
cp ../xcode/ios/Icon-Small.png sdk/hosts/xcode-ios/Icon-Small.png
cp ../xcode/ios/Icon-Small@2x.png sdk/hosts/xcode-ios/Icon-Small@2x.png
cp ../xcode/ios/Icon-Small-50.png sdk/hosts/xcode-ios/Icon-Small-50.png
cp ../xcode/ios/Info.plist sdk/hosts/xcode-ios/Info.plist
cp ../xcode/ios/MainWindow-iPad.xib sdk/hosts/xcode-ios/MainWindow-iPad.xib
cp ../xcode/ios/MainWindow-iPhone.xib sdk/hosts/xcode-ios/MainWindow-iPhone.xib

# binaries
