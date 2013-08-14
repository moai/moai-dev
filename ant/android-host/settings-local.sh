#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

#----------------------------------------------------------------#
# path to Android SDK folder (on Windows, you MUST use forward 
# slashes as directory separators, e.g. C:/android-sdk)
#----------------------------------------------------------------#

	android_sdk_root="/Users/lordmortis/Applications/Android-SDK"

#----------------------------------------------------------------#
# space-delimited list of source lua directories to add to the 
# application bundle and corresponding destination directories in 
# the assets directory of the bundle
#----------------------------------------------------------------#

	src_dirs=( "/Users/lordmortis/Projects/StirFire/BuildSystem2/build-dirs/freedomfall/builds/android" )
	dest_dirs=(	"lua" )

#----------------------------------------------------------------#
# debug & release settings
# you must define key store data in order to build for release
#----------------------------------------------------------------#

	debug=false
	ouya_store_key="/Users/lordmortis/Projects/StirFire/Admin (Sync)/OUYA/Freedom Fall Key.der"
	key_store="/Users/lordmortis/BTSync/Crypt/StirFire/Android-Store.keystore"
	key_alias=key
	key_store_password='^1A0t$x6aIvMStmM'
	key_alias_password='^1A0t$x6aIvMStmM'