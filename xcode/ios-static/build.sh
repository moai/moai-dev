#!/bin/bash

xcodebuild -configuration Release -project MoaiSample.xcodeproj -target moai -sdk iphonesimulator || exit 1
xcodebuild -configuration Release -project MoaiSample.xcodeproj -target moai -sdk iphoneos || exit 1