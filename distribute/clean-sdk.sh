#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

#----------------------------------------------------------------#
# clean main folders
#----------------------------------------------------------------#
rm -rf moai-sdk/3rdparty
rm -rf moai-sdk/docs
rm -rf moai-sdk/include
rm -rf moai-sdk/samples
rm -rf moai-sdk/flash
rm -rf moai-sdk/include

#----------------------------------------------------------------#
# clean version text file
#----------------------------------------------------------------#
rm -f moai-sdk/version.txt

#----------------------------------------------------------------#
# clean host shared src
#----------------------------------------------------------------#
rm -rf moai-sdk/hosts/src

#----------------------------------------------------------------#
# clean eclipse host
#----------------------------------------------------------------#
rm -rf moai-sdk/hosts/eclipse/android-project

#----------------------------------------------------------------#
# clean xcode host
#----------------------------------------------------------------#
rm -rf moai-sdk/hosts/xcode/ios/Classes
rm -f moai-sdk/hosts/xcode/ios/Entitlements.plist
rm -f moai-sdk/hosts/xcode/ios/Icon.png
rm -f moai-sdk/hosts/xcode/ios/Icon@2x.png
rm -f moai-sdk/hosts/xcode/ios/Icon-72.png
rm -f moai-sdk/hosts/xcode/ios/Icon-Small.png
rm -f moai-sdk/hosts/xcode/ios/Icon-Small@2x.png
rm -f moai-sdk/hosts/xcode/ios/Icon-Small-50.png
rm -f moai-sdk/hosts/xcode/ios/Info.plist
rm -f moai-sdk/hosts/xcode/ios/MainWindow-iPad.xib
rm -f moai-sdk/hosts/xcode/ios/MainWindow-iPhone.xib
rm -f moai-sdk/hosts/xcode/ios/main.mm
rm -f moai-sdk/hosts/xcode/ios/mt.default
rm -f moai-sdk/hosts/xcode/ios/package.sh
