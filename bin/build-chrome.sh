#!/bin/bash
export NACL_SDK_ROOT=/Chrome/nacl-sdk
export FMOD_CHROME_SDK_ROOT=/FMOD/Chrome
cd `dirname $0`/..
cd scons
./clean.sh
./build.sh

cd ..
rm -rf release/chrome
mkdir -p release/chrome/lib
cp scons/moai.nmf release/chrome/lib
cp scons/moai_x86_32.nexe release/chrome/lib
cp scons/moai_x86_64.nexe release/chrome/lib
