#!/bin/bash
xcodebuild -scheme SCHEME_NAME -arch i386  -sdk iphonesimulator9.0 -destination 'platform=iphonesimulator,name=iPhone 6 Plus' -derivedDataPath build
ios-sim launch build/Debug-iphonesimulator/*.app
