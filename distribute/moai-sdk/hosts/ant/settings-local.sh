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

	android_sdk_root=""

#----------------------------------------------------------------#
# space-delimited list of source lua directories to add to the 
# application bundle and corresponding destination directories in 
# the assets directory of the bundle
#----------------------------------------------------------------#

	src_dirs=( "../../samples/anim/anim-basic" "../../include/lua-modules" )
	dest_dirs=(	"lua" "lua/lua-modules" )

#----------------------------------------------------------------#
# debug & release settings
# you must define key store data in order to build for release
#----------------------------------------------------------------#

	debug=true
	key_store=
	key_alias=
	key_store_password=
	key_alias_password=