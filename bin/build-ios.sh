#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#



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



usage() {
    cat >&2 <<EOF
usage: $0
     <library destination>
EOF
    exit 1
}

#clean up xcodebuild if we have xcpretty
if hash xcpretty 2>/dev/null; then
 pretty() {
   xcpretty -c
 }
else
 pretty() {
   cat
 } 
fi 

#our build command
#ARCHS, SDK
build () {
  xcodebuild ONLY_ACTIVE_ARCH=NO ARCHS="$1" -project moai.xcodeproj -target install -configuration Release -target install -sdk $2 | pretty
}


if [ x$1 == x ]; then
  libprefix=`dirname $0`/../lib/ios
else
  libprefix=$1
fi


lib_dir=$(ruby -e 'puts File.expand_path(ARGV.first)' "$libprefix")

if ! [ -d $lib_dir ]
then
mkdir -p $lib_dir
fi


#get some required variables
XCODEPATH=$(xcode-select --print-path)


# echo message about what we are doing
echo "Building lib moai via CMAKE"

build_dir=${PWD}

cd `dirname $0`/..

moai_root=$(pwd)

if ! [ -d "build" ]
then
mkdir build
fi
cd build

if ! [ -d "build-ios" ]
then
mkdir build-ios
fi
cd build-ios


echo "Creating xcode project"

set -e

#if ! [ -e "CMakeCache.txt" ]
#then
#create our makefiles
cmake \
-DBUILD_IOS=true \
-DCMAKE_BUILD_TYPE=Release \
-DMOAI_APPLE=TRUE \
-DMOAI_LUAJIT=False \
-DCMAKE_INSTALL_PREFIX="${lib_dir}" \
-G "Xcode" \
$moai_root/cmake
#fi

rm -f ${lib_dir}/lib/*.a


build "i386 x86_64" iphonesimulator
#work around cmake install bug with ios projects
find . -iregex ".*/.*-iphonesimulator/[^/]*.a" | xargs -J % cp -npv % ${lib_dir}/lib


mkdir -p ${lib_dir}/lib-iphonesimulator
mv -v ${lib_dir}/lib/*.a ${lib_dir}/lib-iphonesimulator

rm -f ${lib_dir}/lib/*.a


build "armv7 armv7s arm64" iphoneos
#work around cmake install bug with ios projects
find . -iregex ".*/.*-iphoneos/[^/]*.a" | xargs -J % cp -npv % ${lib_dir}/lib
find . -iregex ".*/Export/cmake/[^/]*.cmake" | xargs -J % cp -pv % ${lib_dir}/cmake

mkdir -p ${lib_dir}/lib-iphoneos
set +e
cp -npv ${lib_dir}/lib/*.a ${lib_dir}/lib-iphoneos

echo "Build Directory : ${build_dir}"
echo "Lib Output: ${lib_dir}"

#todo create lipo and dump in release folder.
for LIBNAME in ${lib_dir}/lib-iphoneos/*.a
do
    BASELIBNAME=`basename $LIBNAME`
    (lipo -info ${lib_dir}/lib-iphoneos/$BASELIBNAME | grep i386) && continue
    echo lip $LIBNAME
    lipo ${lib_dir}/lib-iphoneos/$BASELIBNAME ${lib_dir}/lib-iphonesimulator/$BASELIBNAME -create -output ${lib_dir}/lib/$BASELIBNAME
done


rm -rf ${lib_dir}/lib-iphoneos
rm -rf ${lib_dir}/lib-iphonesimulator
