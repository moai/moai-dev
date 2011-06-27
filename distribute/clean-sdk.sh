#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

#----------------------------------------------------------------#
# clean main folders
#----------------------------------------------------------------#
rm -rf sdk/3rdparty
rm -rf sdk/bin
rm -rf sdk/docs
rm -rf sdk/include

#----------------------------------------------------------------#
# clean version text file
#----------------------------------------------------------------#
rm -f sdk/version.txt

#----------------------------------------------------------------#
# clean lua samples
#----------------------------------------------------------------#
rm -rf sdk/samples/lua

#----------------------------------------------------------------#
# clean host shared src
#----------------------------------------------------------------#
rm -rf sdk/samples/hosts/src

#----------------------------------------------------------------#
# clean eclipse host
#----------------------------------------------------------------#
rm -rf sdk/samples/hosts/eclipse/android-project

#----------------------------------------------------------------#
# clean xcode host
#----------------------------------------------------------------#
rm -rf sdk/samples/hosts/xcode/ios/Classes
rm -f sdk/samples/hosts/xcode/ios/Entitlements.plist
rm -f sdk/samples/hosts/xcode/ios/Icon.png
rm -f sdk/samples/hosts/xcode/ios/Icon@2x.png
rm -f sdk/samples/hosts/xcode/ios/Icon-72.png
rm -f sdk/samples/hosts/xcode/ios/Icon-Small.png
rm -f sdk/samples/hosts/xcode/ios/Icon-Small@2x.png
rm -f sdk/samples/hosts/xcode/ios/Icon-Small-50.png
rm -f sdk/samples/hosts/xcode/ios/Info.plist
rm -f sdk/samples/hosts/xcode/ios/MainWindow-iPad.xib
rm -f sdk/samples/hosts/xcode/ios/MainWindow-iPhone.xib
