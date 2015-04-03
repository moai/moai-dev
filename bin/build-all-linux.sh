#!/bin/bash

set -e

cd `dirname $0`/..

export PATH=$PATH:$(pwd)/util

echo "Building linux libs"

./bin/build-linux.sh

echo "Linux Libs complete"

echo "Creating linux host"

moaiutil host-linux

echo "Linux host Complete"

echo "Building android libs"

./bin/build-android.sh

echo "Android libs complete"

echo "Creating android host"

moaiutil host-android-gradle

echo "Android host Complete"

echo "All linux compatible hosts and libs have been built"