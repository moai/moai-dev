#!/bin/bash
set -e

echo "PREPARE SDK - OSX"
pushd $(dirname "${0}")/../../ > /dev/null

MOAI_SDK_ROOT=$(pwd)
echo $MOAI_SDK_ROOT

#cd $MOAI_SDK_ROOT/ant
#./libmoai-clean.sh
#./libmoai-build.sh

cd $MOAI_SDK_ROOT/xcode/libmoai
./build-ios.sh
./build-osx.sh

popd > /dev/null
