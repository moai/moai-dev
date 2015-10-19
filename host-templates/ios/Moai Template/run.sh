#!/bin/bash
xcodebuild -destination 'platform=iphonesimulator,name=iPhone 6 Plus' -derivedDataPath build
ios-sim launch build/Debug-iphonesimulator/*.app
