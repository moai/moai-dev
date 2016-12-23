#!/bin/bash
set -e

echo "PREPARE SDK - OSX"
pushd $(dirname "${0}")/../../ > /dev/null

MOAI_SDK_ROOT=$(pwd)
echo $MOAI_SDK_ROOT

#cd $MOAI_SDK_ROOT/ant
#./libmoai-clean.sh
#./libmoai-build.sh

cd $MOAI_SDK_ROOT/util/build
./build-ios.sh || exit 1
./build-osx.sh || exit 1

popd > /dev/null
