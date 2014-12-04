#!/bin/bash

xcodebuild -configuration Release -project Moai.xcodeproj -target moai || exit 1
