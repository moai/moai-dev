#!/usr/bin/env bash
echo "Building Moai-SDK for linux - CI"

echo "Telling the OSX build to get busy"
git fetch origin travis-osx:travis-osx
git checkout travis-osx
git merge master
git push https://${GH_TOKEN}@github.com/moaiforge/moai-sdk.git travis-osx:travis-osx > /dev/null
git checkout master

echo "Fetching Latest CMake"
sudo apt-add-repository ppa:kubuntu-ppa/backports -y
sudo apt-get update
sudo apt-get install cmake

echo "Fetching build deps"
sudo apt-get install freeglut3-dev libglew-dev libglu1-mesa-dev libxmu-dev libxi-dev libxxf86vm-dev

echo "Performing build"
pushd .
./bin/build-linux-sdl.sh
if [[ $? -ne 0 ]]; then
  echo "Build failed!" 
  exit 1
fi

popd
