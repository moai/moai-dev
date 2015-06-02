#!/bin/bash

set -e # exit on error

xcodebuild -derivedDataPath build -configuration Release -project Moai.xcodeproj -scheme moai
