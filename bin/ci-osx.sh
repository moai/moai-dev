#!/usr/bin/env bash
echo "Installing CMake 2.8.12"
pushd .
cd $TMPDIR
curl http://www.cmake.org/files/v2.8/cmake-2.8.12.2-Darwin64-universal.tar.gz -o cmake.tar.gz
tar zxvf cmake.tar.gz
export PATH=$PATH:$TMPDIR/cmake-2.8.12.2-Darwin64-universal/CMake\ 2.8-12.app/Contents/bin
popd                          

echo "Building OSX"
pushd .

./bin/build-osx-sdl.sh
