#!/bin/bash

set -e # exit on error

pushd ../libmoai/ > /dev/null
./build-osx.sh
popd > /dev/null

xcodebuild -derivedDataPath build -configuration Release -project Moai.xcodeproj -scheme moai
