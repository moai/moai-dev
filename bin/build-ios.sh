#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

# Set this to a name which will be valid for "codesign -s", or the build
# will fail.
SIGN_IDENTITY='iPhone Developer'

codesign_msg=$(codesign -s "$SIGN_IDENTITY" 2>&1)
case $codesign_msg in
*": no identity found"*)
    echo >&2 "Code-signing identity ($SIGN_IDENTITY) is invalid."
    exit 1
    ;;
esac

set -e

# Give more useful feedback rather than aborting silently.
report_error() {
    status=$?
    case $status in
    0)    ;;
    *)    echo >&2 "Aborting due to exit status $status from: $BASH_COMMAND";;
    esac
    exit $status
}

trap 'report_error' 0

APP_NAME='Moai App'
APP_ID='com.getmoai.moaiapp'
APP_VERSION='1.0'

usage() {
    cat >&2 <<EOF
usage: $0
    [--use-untz true | false] [--disable-adcolony] [--disable-billing]
    [--disable-chartboost] [--disable-crittercism] [--disable-facebook]
    [--disable-mobileapptracker] [--disable-push] [--disable-tapjoy]
    [--disable-twitter] [--simulator] [--release]
    <lua-src-path>
EOF
    exit 1
}

# check for command line switches
use_untz="true"

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
simulator="false"

while [ $# -gt 0 ];	do
    case "$1" in
        --use-untz)  use_untz="$2"; shift;;
        --disable-adcolony)  adcolony_flags="-DDISABLE_ADCOLONY";;
        --disable-billing)  billing_flags="-DDISABLE_BILLING";;
        --disable-chartboost)  chartboost_flags="-DDISABLE_CHARTBOOST";;
        --disable-crittercism)  crittercism_flags="-DDISABLE_CRITTERCISM";;
        --disable-facebook)  facebook_flags="-DDISABLE_FACEBOOK";;
        --disable-mobileapptracker)  mobileapptracker_flags="-DDISABLE_MOBILEAPPTRACKER";;
        --disable-push)  push_flags="-DDISABLE_NOTIFICATIONS";;
        --disable-tapjoy)  tapjoy_flags="-DDISABLE_TAPJOY";;
        --disable-twitter)  twitter_flags="-DDISABLE_TWITTER";;
        --release) buildtype_flags="Release";;
        --simulator) simulator="true";;
        -*) usage;;
        *)  break;;	# terminate while loop
    esac
    shift
done

if [ $# != 1 ]; then
    usage
fi

LUASRC=$(ruby -e 'puts File.expand_path(ARGV.first)' "$1")

if [ ! -f "${LUASRC}/main.lua" ]; then
    echo -n "Please enter the directory path of the Lua source. > "
    read LUASRC
    LUASRC=$(ruby -e 'puts File.expand_path(ARGV.first)' "$LUASRC")

    if [ ! -f "${LUASRC}/main.lua" ]; then
        echo "Could not find main.lua in specified lua source directory [${LUASRC}]"
        exit 1
    fi
fi

if [ x"$use_untz" != xtrue ] && [ x"$use_untz" != xfalse ]; then
    usage
fi

#get some required variables
XCODEPATH=$(xcode-select --print-path)

if [ x"$simulator" == xtrue ]; then
echo "RUNNING SIMULATOR $simulator"
PLATFORM_PATH=${XCODEPATH}/Platforms/iPhoneSimulator.platform/Developer
PLATFORM=SIMULATOR
SDK=iphonesimulator
ARCH=i386
else
PLATFORM_PATH=${XCODEPATH}/Platforms/iPhone.platform/Developer
PLATFORM=OS
SDK=iphoneos
ARCH=armv7
fi

# echo message about what we are doing
echo "Building moai.app via CMAKE"

disabled_ext=
    
if [ x"$use_untz" != xtrue ]; then
    echo "UNTZ will be disabled"
    untz_param='-DMOAI_UNTZ=0'
else
    untz_param='-DMOAI_UNTZ=1'
fi 

if [ x"$adcolony_flags" != x ]; then
    echo "AdColony will be disabled"
    disabled_ext="${disabled_ext}ADCOLONY;"
fi 

if [ x"$billing_flags" != x ]; then
    echo "Billing will be disabled"
    disabled_ext="${disabled_ext}BILLING;"
fi 

if [ x"$chartboost_flags" != x ]; then
    echo "ChartBoost will be disabled"
    disabled_ext="${disabled_ext}CHARTBOOST;"
fi 

if [ x"$crittercism_flags" != x ]; then
    echo "Crittercism will be disabled"
    disabled_ext="${disabled_ext}CRITTERCISM;"
fi 

if [ x"$facebook_flags" != x ]; then
    echo "Facebook will be disabled"
    disabled_ext="${disabled_ext}FACEBOOK;"
fi 

if [ x"$mobileapptracker_flags" != x ]; then
    echo "Mobile App Tracker will be disabled"
    disabled_ext="${disabled_ext}MOBILEAPPTRACKER;"
fi 

if [ x"$push_flags" != x ]; then
    echo "Push Notifications will be disabled"
    disabled_ext="${disabled_ext}NOTIFICATIONS;"
fi 

if [ x"$tapjoy_flags" != x ]; then
    echo "Tapjoy will be disabled"
    disabled_ext="${disabled_ext}TAPJOY;"
fi 

if [ x"$twitter_flags" != x ]; then
    echo "Twitter will be disabled"
    disabled_ext="${disabled_ext}TWITTER;"
fi 

build_dir=${PWD}

cd `dirname $0`/..
cd cmake
rm -rf build
mkdir build
cd build

echo "Building resource list from ${LUASRC}"
ruby ../host-ios/build_resources.rb "${LUASRC}"

echo "Creating xcode project"

#create our makefiles
cmake -DDISABLED_EXT="$disabled_ext" -DMOAI_BOX2D=1 \
-DMOAI_CHIPMUNK=1 -DMOAI_CURL=1 -DMOAI_CRYPTO=0 -DMOAI_EXPAT=1 -DMOAI_FREETYPE=1 \
-DMOAI_HTTP_CLIENT=1 -DMOAI_JSON=1 -DMOAI_JPG=1 -DMOAI_LUAEXT=1 \
-DMOAI_MONGOOSE=1 -DMOAI_OGG=1 -DMOAI_OPENSSL=0 -DMOAI_SQLITE3=1 \
-DMOAI_TINYXML=1 -DMOAI_PNG=1 -DMOAI_SFMT=1 -DMOAI_VORBIS=1 $untz_param \
-DMOAI_LUAJIT=1 \
-DBUILD_IOS=true \
-DSIGN_IDENTITY="${SIGN_IDENTITY}" \
-DAPP_NAME="${APP_NAME}" \
-DAPP_ID="${APP_ID}" \
-DAPP_VERSION="${APP_VERSION}" \
-DCMAKE_BUILD_TYPE=$buildtype_flags \
-G "Xcode" \
../

#-DCMAKE_TOOLCHAIN_FILE="${PWD}/../host-ios/iOS.cmake" \
#-DCMAKE_IOS_DEVELOPER_ROOT="${PLATFORM_PATH}" \
      #build them
xcodebuild -target moai -sdk ${SDK} -arch ${ARCH}

echo "Build Directory : ${build_dir}"

# Copy libs
cd `dirname $0`/..
if [ -d "release/ios" ]; then
    rm -fr release/ios
fi

mkdir -p release/ios/app
mkdir -p release/ios/lib

find cmake/build -name "*.app" | xargs -J % cp -fRp % release/ios/app
find cmake/build -name "*.a" | xargs -J % cp -fp % release/ios/lib
