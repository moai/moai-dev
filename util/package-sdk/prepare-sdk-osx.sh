#!/bin/bash
set -e

echo "PREPARE SDK - OSX"
pushd $(dirname "${0}")/../../ > /dev/null

MOAI_SDK_ROOT=$(pwd)
echo $MOAI_SDK_ROOT

cd $MOAI_SDK_ROOT/ant
#./libmoai-clean.sh
./libmoai-build.sh

cd $MOAI_SDK_ROOT/xcode/libmoai

#./clean-ios.sh
./build-ios.sh

#./clean-osx.sh
./build-osx.sh

#cd $MOAI_SDK_ROOT/xcode/ios-static

#./clean.sh
./build.sh

cd $MOAI_SDK_ROOT/xcode/osx-static

#./clean.sh
./build.sh

popd > /dev/null
