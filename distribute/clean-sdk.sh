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
rm -rf sdk/samples
rm -rf sdk/include

#----------------------------------------------------------------#
# clean version text file
#----------------------------------------------------------------#
rm -f sdk/version.txt

#----------------------------------------------------------------#
# clean host shared src
#----------------------------------------------------------------#
rm -rf sdk/hosts/src

#----------------------------------------------------------------#
# clean eclipse host
#----------------------------------------------------------------#
rm -rf sdk/hosts/eclipse/android-project

#----------------------------------------------------------------#
# clean xcode host
#----------------------------------------------------------------#
rm -rf sdk/hosts/xcode/ios/Classes
rm -f sdk/hosts/xcode/ios/Entitlements.plist
rm -f sdk/hosts/xcode/ios/Icon.png
rm -f sdk/hosts/xcode/ios/Icon@2x.png
rm -f sdk/hosts/xcode/ios/Icon-72.png
rm -f sdk/hosts/xcode/ios/Icon-Small.png
rm -f sdk/hosts/xcode/ios/Icon-Small@2x.png
rm -f sdk/hosts/xcode/ios/Icon-Small-50.png
rm -f sdk/hosts/xcode/ios/Info.plist
rm -f sdk/hosts/xcode/ios/MainWindow-iPad.xib
rm -f sdk/hosts/xcode/ios/MainWindow-iPhone.xib
