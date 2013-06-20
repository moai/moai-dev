#!/bin/bash

# Automatically builds, installs, and runs automated testing on a device connected by USB.
# Requires fruitstrap: recommended fork: https://github.com/kenji21/fruitstrap

$DSTROOT=/tmp/ExampleKIF

set -o errexit
set -o verbose

# Build the "Integration Tests" target to run on a device
xcodebuild -target "Example (KIF)" -configuration Release clean build

# install and run the app on a device connected by USB
fruitstrap install -d --bundle "build/Release-iphoneos/KIFExample.app" > console.log 2>&1

# now show the output to the console
cat console.log
