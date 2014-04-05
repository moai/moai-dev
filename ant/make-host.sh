#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

set -e

cd `dirname $0`/

usage="usage: $0 [-p <package>] [-s] [-i thumb | arm] [-a all | armeabi | armeabi-v7a] [-l appPlatform] [--use-fmod \
    true | false] [--use-untz true | false] [--use-luajit true | false] [--disable-adcolony] [--disable-billing] \
    [--disable-chartboost] [--disable-crittercism] [--disable-facebook] [--disable-push] [--disable-tapjoy] \
    [--disable-twitter] [--windows]"
skip_build="false"
package_name="com.getmoai.samples"
arm_mode="arm"
arm_arch="armeabi-v7a"
app_platform="android-10"
use_fmod="false"
use_untz="true"
use_luajit="true"
adcolony_flags=
billing_flags=
chartboost_flags=
crittercism_flags=
facebook_flags=
push_flags=
tapjoy_flags=
twitter_flags=
windows_flags=

while [ $# -gt 0 ];	do
    case "$1" in
        -s)  skip_build="true";;
        -p)  package_name="$2"; shift;;
        -i)  arm_mode="$2"; shift;;
        -a)  arm_arch="$2"; shift;;
        -l)  app_platform="$2"; shift;;
        --use-fmod)  use_fmod="$2"; shift;;
        --use-untz)  use_untz="$2"; shift;;
        --use-luajit)  use_luajit="$2"; shift;;
        --disable-adcolony)  adcolony_flags="--disable-adcolony";;
        --disable-billing)  billing_flags="--disable-billing";;
        --disable-chartboost)  chartboost_flags="--disable-chartboost";;
        --disable-crittercism)  crittercism_flags="--disable-crittercism";;
        --disable-facebook)  facebook_flags="--disable-facebook";;
        --disable-push)  push_flags="--disable-push";;
        --disable-tapjoy)  tapjoy_flags="--disable-tapjoy";;
        --disable-twitter)  twitter_flags="--disable-twitter";;
        --windows) windows_flags="--windows";;
        -*)
            echo >&2 \
                $usage
            exit 1;;
        *)  break;;
    esac
    shift
done	
    
if [ x"$package_name" == x ]; then
    echo $usage
    exit 1
fi

if [ x"$arm_mode" != xarm ] && [ x"$arm_mode" != xthumb ]; then
    echo $usage
    exit 1
fi

if [ x"$arm_arch" != xarmeabi ] && [ x"$arm_arch" != xarmeabi-v7a ] && [ x"$arm_arch" != xall ]; then
    echo $usage
    exit 1
fi

# TODO: Validate app_platform

if [ x"$use_fmod" != xtrue ] && [ x"$use_fmod" != xfalse ]; then
    echo $usage
    exit 1
fi

if [ x"$use_untz" != xtrue ] && [ x"$use_untz" != xfalse ]; then
    echo $usage
    exit 1
fi

if [ x"$use_luajit" != xtrue ] && [ x"$use_luajit" != xfalse ]; then
    echo $usage
    exit 1
fi


if [ x"$use_fmod" == xtrue ] && [ x"$FMOD_ANDROID_SDK_ROOT" == x ]; then
    echo "*** The FMOD SDK is not redistributed with the Moai SDK. Please download the FMOD EX"
    echo "*** Programmers API SDK from http://fmod.org and install it. Then ensure that the"
    echo "*** FMOD_ANDROID_SDK_ROOT environment variable is set and points to the root of the"
    echo "*** FMOD SDK installation; e.g., /FMOD/Android"
    exit 1		
fi

new_host_dir="`pwd`/untitled-host"
if [ -d $new_host_dir ]; then
    rm -rf $new_host_dir
fi

if [ x"$skip_build" != xtrue ]; then
    pushd libmoai > /dev/null
        bash build.sh --use-untz $use_untz --use-luajit $use_luajit  \
            $adcolony_flags $billing_flags $chartboost_flags $crittercism_flags \
            $facebook_flags $push_flags $tapjoy_flags $twitter_flags $windows_flags 
    popd > /dev/null
fi

new_host_lib_dir=$new_host_dir/host-source/project/libs
mkdir -p $new_host_lib_dir

if [ -d libmoai/libs ]; then
    cp -fR libmoai/libs/* $new_host_lib_dir
    rm -f $new_host_lib_dir/package.txt
else
    echo "*** libmoai has not been built. This host will be incomplete until a version"
    echo "*** of libmoai.so is placed in $new_host_dir/host-source/project/libs/armeabi"
    echo "*** or $new_host_dir/host-source/project/libs/armeabi-v7a."
    echo ""
fi

backup_ext=.backup	
function fr () { 
    sed -i$backup_ext s%"$2"%"$3"%g $1
    rm -f $1$backup_ext
}
    
required_libs="\"miscellaneous\""

if [ x"$adcolony_flags" == x ]; then
    required_libs="$required_libs \"adcolony\""
fi

if [ x"$billing_flags" == x ]; then
    required_libs="$required_libs \"google-billing\""
fi

if [ x"$chartboost_flags" == x ]; then
    required_libs="$required_libs \"chartboost\""
fi

if [ x"$crittercism_flags" == x ]; then
    required_libs="$required_libs \"crittercism\""
fi

if [ x"$facebook_flags" == x ]; then
    required_libs="$required_libs \"facebook\""
fi

if [ x"$push_flags" == x ]; then
    required_libs="$required_libs \"google-push\""
fi

if [ x"$tapjoy_flags" == x ]; then
    required_libs="$required_libs \"tapjoy\""
fi

if [ x"$twitter_flags" == x ]; then
    required_libs="$required_libs \"twitter\""
fi

cp -f host-source/d.settings-local.sh $new_host_dir/settings-local.sh
cp -f host-source/d.settings-global.sh $new_host_dir/settings-global.sh
fr $new_host_dir/settings-global.sh @REQUIRED_LIBS@ "$required_libs"

cp -f host-source/d.README.txt $new_host_dir/README.txt
cp -f host-source/d.run-host.sh $new_host_dir/run-host.sh
cp -f host-source/d.run-host.bat $new_host_dir/run-host.bat	

pushd host-source/d.res > /dev/null
    find . -name ".?*" -type d -prune -o -print0 | cpio -pmd0 --quiet $new_host_dir/res
popd > /dev/null	

pushd host-source/source > /dev/null
    find . -name ".?*" -type d -prune -o -name "src" -type d -prune -o -name "external" -type d -prune -o -type f -print0 | cpio -pmd0 --quiet $new_host_dir/host-source
popd > /dev/null

mkdir -p $new_host_dir/host-source/project/src/com/ziplinegames/moai
OLD_IFS=$IFS
IFS='.'
package_path=src
for word in $package_name; do 
    package_path=$package_path/$word
    mkdir -p $new_host_dir/host-source/project/$package_path
done
IFS=$OLD_IFS

pushd host-source/source/project/src/app > /dev/null
    find . -name ".?*" -type d -prune -o -type f -print0 | cpio -pmd0 --quiet $new_host_dir/host-source/project/$package_path
popd > /dev/null

pushd host-source/source/project/src/moai > /dev/null
    find . -name ".?*" -type d -prune -o -mindepth 2 -type f -print0 | cpio -pmd0 --quiet $new_host_dir/host-source/moai
popd > /dev/null

pushd host-source/source/project/src/moai > /dev/null
    find . -name ".?*" -type d -prune -o -maxdepth 1 -type f -print0 | cpio -pmd0 --quiet $new_host_dir/host-source/project/src/com/ziplinegames/moai
popd > /dev/null

pushd host-source/source/project/external > /dev/null
    find . -name ".?*" -type d -prune -o -type f -print0 | cpio -pmd0 --quiet $new_host_dir/host-source/external
popd > /dev/null

for file in `find $new_host_dir/host-source/ -name "project.properties"` ; do fr $file @APP_PLATFORM@ "$app_platform" ; done

sed -i.backup s%@SETTING_PACKAGE_PATH@%"$package_path"%g $new_host_dir/run-host.sh
rm -f $new_host_dir/run-host.sh.backup

sed -i.backup s%@SETTING_PACKAGE@%"$package_name"%g $new_host_dir/run-host.sh
rm -f $new_host_dir/run-host.sh.backup

sed -i.backup s%@SETTING_PACKAGE@%"$package_name"%g $new_host_dir/run-host.bat
rm -f $new_host_dir/run-host.bat.backup

echo "********************************************************************************"
echo "* Android host successfully created.                                           *"
echo "********************************************************************************"
echo ""
echo "- The new host is in the \"untitled-host\" directory. Every time this script is"
echo "  run, it will clobber the contents of the \"untitled-host\" directory so you"
echo "  will probably want to move this folder elsewhere and rename it."
echo ""
echo "- Edit \"settings-global.sh\" to configure your project name and version info,"
echo "  required optional components, icon resources and the list of lua files to run"
echo "  when the application starts. This file is globally applicable to any users of"
echo "  the host and therefore may be checked into version control."
echo ""
echo "- Edit \"settings-local.sh\" to configure the path to your Android SDK and signing"
echo "  keys, as well as the paths to your lua source that need to be copies into the"
echo "  application bundle. Because this file contains machine-specific paths, it is"
echo "  probably not appropriate to check into version control - each user of the host"
echo "  will need their own \"settings-local.sh\" file configured appropriately."
echo ""
echo "- Note that host is tied to the package name you specified when it was created."
echo "  If you wish to change the package name, simply recreate the host with the new"
echo "  package name and re-apply your settings files (and any resource files) into the"
echo "  new project."
echo ""
echo "  WARNING: EVERY time this script is run, the contents of \"untitled-host\" will"
echo "  be obliterated!"
echo ""
echo "********************************************************************************"
	
