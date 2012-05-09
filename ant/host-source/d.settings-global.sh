#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

#----------------------------------------------------------------#
# application and project names
#----------------------------------------------------------------#

	project_name="untitled"
	app_name="untitled"

#----------------------------------------------------------------#
# version numbers
#----------------------------------------------------------------#

	version_code="1"
	version_name="1.0"

#----------------------------------------------------------------#
# space-delimited list of libraries (moai-supported) required 
# (this list is created by make-host.sh using command-line 
# information)
# available: facebook, tapjoy, crittercism, google-push, 
#            google-billing, miscellaneous (required by google-billing)
#----------------------------------------------------------------#

	requires=( @REQUIRED_LIBS@ )
		
#----------------------------------------------------------------#
# list of icon files
#----------------------------------------------------------------#

	icon_ldpi="res/icon-ldpi.png"
	icon_mdpi="res/icon-mdpi.png"
	icon_hdpi="res/icon-hdpi.png"
	icon_xhdpi="res/icon-xhdpi.png"
		
#----------------------------------------------------------------#
# working directory in the assets directory of the application 
# bundle and a space-delimited list of lua files thereunder to run 
# when the application starts
#----------------------------------------------------------------#

	working_dir="lua"
	run=( "main.lua" )