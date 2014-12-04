#!/bin/bash

MOAI_SDK_ROOT=$(dirname "${0}")/../../

echo "PREPARE SDK - OSX"
pushd $(dirname "${0}")/../../ > /dev/null

MOAI_SDK_ROOT=$(pwd)
echo $MOAI_SDK_ROOT

#moaiutil make-lua-docs -o "distribute/html"

cd $MOAI_SDK_ROOT/ant/libmoai/jni
ndk-build || exit 1

cd $MOAI_SDK_ROOT/xcode/libmoai
./build.sh || exit 1

cd $MOAI_SDK_ROOT/xcode/ios
./build.sh || exit 1

cd $MOAI_SDK_ROOT/xcode/ios-static
./build.sh || exit 1

cd $MOAI_SDK_ROOT/xcode/osx
./build.sh || exit 1

cd $MOAI_SDK_ROOT/xcode/osx-static
./build.sh || exit 1

popd > /dev/null
