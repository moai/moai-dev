#!/bin/bash

set -e


cd `dirname $0`
MOAIROOT=$(pwd)/..

cd $MOAIROOT



echo "Building osx libs"

./bin/build-osx.sh

echo "OSX Libs complete"

echo "Creating osx host"

moaiutil host-osx-app

echo "OSX host Complete"

echo "Building ios libs"

./bin/build-ios.sh

echo "iOS libs complete"

echo "Creating iOS Host"

moaiutil host-ios

echo "iOS host complete"

echo "Building android libs"

./bin/build-android.sh

echo "Android libs complete"

echo "Creating android host"

moaiutil host-android-gradle

echo "Android host Complete"

echo "Building html lib"

./bin/build-html.sh

echo "Creating html lib complete"

echo "Creating html host"

moaiutil host-html

echo "Html host complete"


echo "All OSX compatible hosts and libs have been built"