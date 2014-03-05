#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

set -e

cd `dirname $0`/

if [ x"$ANDROID_NDK" == x ]; then
    echo "ANDROID_NDK not defined. Please set to the location of your Android NDK install (path)"
    exit 1
fi
# check for command line switches
usage="usage: $0  \
    [--use-untz true | false] [--use-luajit true | false] [--disable-adcolony] [--disable-billing] \
    [--disable-chartboost] [--disable-crittercism] [--disable-facebook] [--disable-push] [--disable-tapjoy] \
    [--disable-twitter] [--windows] [--release]"

use_untz="true"
use_luajit="false"

adcolony_flags=
billing_flags=
chartboost_flags=
crittercism_flags=
facebook_flags=
push_flags=
tapjoy_flags=
twitter_flags=
buildtype_flags="Debug"
windows_flags=

while [ $# -gt 0 ];	do
    case "$1" in
        --use-untz)  use_untz="$2"; shift;;
        --use-luajit)  use_luajit="$2"; shift;;
        --disable-adcolony)  adcolony_flags="-DDISABLE_ADCOLONY";;
        --disable-billing)  billing_flags="-DDISABLE_BILLING";;
        --disable-chartboost)  chartboost_flags="-DDISABLE_CHARTBOOST";;
        --disable-crittercism)  crittercism_flags="-DDISABLE_CRITTERCISM";;
        --disable-facebook)  facebook_flags="-DDISABLE_FACEBOOK";;
        --disable-push)  push_flags="-DDISABLE_NOTIFICATIONS";;
        --disable-tapjoy)  tapjoy_flags="-DDISABLE_TAPJOY";;
        --disable-twitter)  twitter_flags="-DDISABLE_TWITTER";;
        --release) buildtype_flags="Release";;
        --windows) windows_flags=-G"MinGW Makefiles";; 
        -*)
            echo >&2 \
                $usage
            exit 1;;
        *)  break;;	# terminate while loop
    esac
    shift
done
make_flags=
if [ x"$windows_flags" != x ]; then
  make_flags=-DCMAKE_MAKE_PROGRAM="${ANDROID_NDK}/prebuilt/windows/bin/make.exe"
fi

if [ x"$use_untz" != xtrue ] && [ x"$use_untz" != xfalse ]; then
    echo $usage
    exit 1		
fi

if [ x"$use_luajit" != xtrue ] && [ x"$use_luajit" != xfalse ]; then
    echo $usage
    exit 1		
fi

should_clean=false

# if libmoai already exists, find out which package it was build for
if [ -f libs/package.txt ]; then
    existing_use_untz=$( sed -n '1p' libs/package.txt )
    existing_use_luajit=$( sed -n '2p' libs/package.txt )
    existing_adcolony_flags=$( sed -n '3p' libs/package.txt )
    existing_billing_flags=$( sed -n '4p' libs/package.txt )
    existing_chartboost_flags=$( sed -n '5p' libs/package.txt )
    existing_crittercism_flags=$( sed -n '6p' libs/package.txt )
    existing_facebook_flags=$( sed -n '7p' libs/package.txt )
    existing_push_flags=$( sed -n '8p' libs/package.txt )
    existing_tapjoy_flags=$( sed -n '9p' libs/package.txt )
    existing_twitter_flags=$( sed -n '10p' libs/package.txt )



    if [ x"$existing_use_untz" != x"$use_untz" ]; then
        should_clean=true
    fi


    if [ x"$existing_adcolony_flags" != x"$adcolony_flags" ]; then
        should_clean=true
    fi

    if [ x"$existing_billing_flags" != x"$billing_flags" ]; then
        should_clean=true
    fi

    if [ x"$existing_chartboost_flags" != x"$chartboost_flags" ]; then
        should_clean=true
    fi

    if [ x"$existing_crittercism_flags" != x"$crittercism_flags" ]; then
        should_clean=true
    fi

    if [ x"$existing_facebook_flags" != x"$facebook_flags" ]; then
        should_clean=true
    fi

    if [ x"$existing_push_flags" != x"$push_flags" ]; then
        should_clean=true
    fi

    if [ x"$existing_tapjoy_flags" != x"$tapjoy_flags" ]; then
        should_clean=true
    fi
    if [ x"$existing_twitter_flags" != x"$twitter_flags" ]; then
        should_clean=true
    fi

fi

if [ x"$should_clean" = xtrue ]; then
    ./clean.sh
fi

# echo message about what we are doing
echo "Building libmoai.so via CMAKE"

disabled_ext=
    


if [ x"$use_untz" != xtrue ]; then
    echo "UNTZ will be disabled"
    untz_param='-DMOAI_UNTZ=0'
else
    untz_param='-DMOAI_UNTZ=1'
fi 

if [ x"$use_luajit" != xtrue ]; then
    echo "LUAJIT will be disabled"
    luajit_param='-DMOAI_LUAJIT=0'
else
    luajit_param='-DMOAI_LUAJIT=1'
fi 

if [ x"$adcolony_flags" != x ]; then
    echo "AdColony will be disabled"
    disabled_ext="$disabled_extADCOLONY;"
fi 

if [ x"$billing_flags" != x ]; then
    echo "Billing will be disabled"
    disabled_ext="$disabled_extBILLING;"
fi 

if [ x"$chartboost_flags" != x ]; then
    echo "ChartBoost will be disabled"
    disabled_ext="$disabled_extCHARTBOOST;"
fi 

if [ x"$crittercism_flags" != x ]; then
    echo "Crittercism will be disabled"
    disabled_ext="$disabled_extCRITTERCISM;"
fi 

if [ x"$facebook_flags" != x ]; then
    echo "Facebook will be disabled"
    disabled_ext="$disabled_extFACEBOOK;"
fi 

if [ x"$push_flags" != x ]; then
    echo "Push Notifications will be disabled"
    disabled_ext="$disabled_extNOTIFICATIONS;"
fi 

if [ x"$tapjoy_flags" != x ]; then
    echo "Tapjoy will be disabled"
    disabled_ext="$disabled_extTAPJOY;"
fi 

if [ x"$twitter_flags" != x ]; then
    echo "Twitter will be disabled"
    disabled_ext="$disabled_extTWITTER;"
fi 

build_dir=${PWD}
cd ../../cmake
rm -rf build
mkdir build
cd build
 
#create our makefiles
cmake -DDISABLED_EXT="$disabled_ext" -DMOAI_BOX2D=1 \
-DMOAI_CHIPMUNK=1 -DMOAI_CURL=1 -DMOAI_CRYPTO=1 -DMOAI_EXPAT=1 -DMOAI_FREETYPE=1 \
-DMOAI_HTTP_CLIENT=1 -DMOAI_JSON=1 -DMOAI_JPG=1 -DMOAI_LUAEXT=1 \
-DMOAI_MONGOOSE=1 -DMOAI_OGG=1 -DMOAI_OPENSSL=1 -DMOAI_SQLITE3=1 \
-DMOAI_TINYXML=1 -DMOAI_PNG=1 -DMOAI_SFMT=1 -DMOAI_VORBIS=1 $untz_param $luajit_param \
-DBUILD_ANDROID=true \
-DCMAKE_TOOLCHAIN_FILE="../host-android/android.toolchain.cmake" \
-DLIBRARY_OUTPUT_PATH_ROOT="../../ant/libmoai" \
-DANDROID_NDK=${ANDROID_NDK}  \
-DCMAKE_BUILD_TYPE=$buildtype_flags \
"${windows_flags}" "${make_flags}" \
../
#build them    
if [ x"$windows_flags" != x ]; then
  cmake --build . --target moai
else
  cmake --build . --target moai -- -j4
fi  


cd ${build_dir}

# create text file that shows the settings libmoai.so was built with (this time)
rm -f libs/package.txt
echo "$use_untz" >> libs/package.txt
echo "$use_luajit" >> libs/package.txt
echo "$adcolony_flags" >> libs/package.txt
echo "$billing_flags" >> libs/package.txt
echo "$chartboost_flags" >> libs/package.txt
echo "$crittercism_flags" >> libs/package.txt
echo "$facebook_flags" >> libs/package.txt
echo "$push_flags" >> libs/package.txt
echo "$tapjoy_flags" >> libs/package.txt
echo "$twitter_flags" >> libs/package.txt
