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

	android_sdk_root="/usr/local/Cellar/android-sdk/22.3/"

#----------------------------------------------------------------#
# space-delimited list of source lua directories to add to the 
# application bundle and corresponding destination directories in 
# the assets directory of the bundle
#----------------------------------------------------------------#

	src_dirs=( "../../samples/android/app-heyzap" )
	dest_dirs=(	"lua" )

#----------------------------------------------------------------#
# Set the orientation of the screen.(landscape, portrait, etc...)
#----------------------------------------------------------------#

	screenOrientation="landscape"

#----------------------------------------------------------------#
# space-delimited list of asset directories to add to the 
# application bundle in the assets directory 
#----------------------------------------------------------------#

	asset_dirs=( )

#----------------------------------------------------------------#
# debug & release settings
# you must define key store data in order to build for release
#----------------------------------------------------------------#

	debug=true
	key_store=
	key_alias=
	key_store_password=
	key_alias_password=