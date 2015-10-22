#!/bin/bash

set -e # exit on error

pushd $(dirname "${0}") > /dev/null

pushd ../libmoai/ > /dev/null
./build-osx.sh
popd > /dev/null

xcodebuild -derivedDataPath build -configuration Release -project Moai.xcodeproj -scheme moai

popd > /dev/null