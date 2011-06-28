#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

./clean-sdk.sh

# general files
cp -R ../../3rdparty/glut-3.7.6-bin sdk/3rdparty/glut-3.7.6/

cp -R ../../samples/basics sdk/samples/lua/
cp -R ../../samples/hello-moai sdk/samples/lua/hello-moai/
cp -R ../../samples/config sdk/samples/lua/config/

cp -R ../../src/aku/*.h sdk/include/aku/

cp -R ../../src/samples sdk/samples/hosts/src

# android host
cp -R ../../eclipse/android-project sdk/samples/hosts/eclipse/android-project/

# ios host
cp -R ../../xcode/ios/Classes sdk/samples/hosts/xcode/ios/Classes
cp ../../xcode/ios/Entitlements.plist sdk/samples/hosts/xcode/ios/Entitlements.plist
cp ../../xcode/ios/Icon.png sdk/samples/hosts/xcode/ios/Icon.png
cp ../../xcode/ios/Icon@2x.png sdk/samples/hosts/xcode/ios/Icon@2x.png
cp ../../xcode/ios/Icon-72.png sdk/samples/hosts/xcode/ios/Icon-72.png
cp ../../xcode/ios/Icon-Small.png sdk/samples/hosts/xcode/ios/Icon-Small.png
cp ../../xcode/ios/Icon-Small@2x.png sdk/samples/hosts/xcode/ios/Icon-Small@2x.png
cp ../../xcode/ios/Icon-Small-50.png sdk/samples/hosts/xcode/ios/Icon-Small-50.png
cp ../../xcode/ios/Info.plist sdk/samples/hosts/xcode/ios/Info.plist
cp ../../xcode/ios/MainWindow-iPad.xib sdk/samples/hosts/xcode/ios/MainWindow-iPad.xib
cp ../../xcode/ios/MainWindow-iPhone.xib sdk/samples/hosts/xcode/ios/MainWindow-iPhone.xib
