#!/bin/bash

MOAI_SDK_ROOT=$(dirname "${0}")/../../

echo "PREPARE SDK - OSX"
pushd $(dirname "${0}")/../../ > /dev/null

MOAI_SDK_ROOT=$(pwd)
echo $MOAI_SDK_ROOT

#moaiutil make-lua-docs -o "distribute/html"

cd $MOAI_SDK_ROOT/xcode/libmoai
./build.sh

cd $MOAI_SDK_ROOT/xcode/ios
./build.sh

cd $MOAI_SDK_ROOT/xcode/ios-static
./build.sh

cd $MOAI_SDK_ROOT/xcode/osx
./build.sh

cd $MOAI_SDK_ROOT/xcode/osx-static
./build.sh

popd > /dev/null
