#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

./clean-sdk.sh

# general files
mkdir -p moai-sdk/3rdparty/glut-3.7.6
cp -R ../3rdparty/glut-3.7.6-bin moai-sdk/3rdparty/glut-3.7.6

mkdir -p moai-sdk/samples
cp -R ../samples/basics moai-sdk/samples

mkdir -p moai-sdk/samples/hello-moai
cp -R ../samples/hello-moai moai-sdk/samples/hello-moai

mkdir -p moai-sdk/samples/config
cp -R ../samples/config moai-sdk/samples/config

mkdir -p moai-sdk/include/aku
cp -R ../src/aku/*.h moai-sdk/include/aku

mkdir -p moai-sdk/hosts/src
cp -R ../src/hosts moai-sdk/hosts/src

# android host
mkdir -p moai-sdk/hosts/eclipse
cp -R ../eclipse moai-sdk/hosts/eclipse/android-project

# ios host
mkdir -p moai-sdk/hosts/xcode-ios/Classes
cp -R ../xcode/ios/Classes moai-sdk/hosts/xcode-ios/Classes

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

