#!/bin/bash

set -e # exit on error

xcodebuild -derivedDataPath build -configuration Release -project MoaiSample.xcodeproj -scheme moai -sdk iphonesimulator
xcodebuild -derivedDataPath build -configuration Release -project MoaiSample.xcodeproj -scheme moai -sdk iphoneos
