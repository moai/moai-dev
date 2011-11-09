#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	project_name="untitled"

#----------------------------------------------------------------#
# debug & release settings
# you must define key store data in order to build for release
#----------------------------------------------------------------#

	debug=true
	key_store=
	key_alias=

#----------------------------------------------------------------#
# list of icon files
#----------------------------------------------------------------#

	icon_ldpi="res/icon-ldpi.png"
	icon_mdpi="res/icon-mdpi.png"
	icon_hdpi="res/icon-hdpi.png"

#----------------------------------------------------------------#
# space-delimited list of source directories, and corresponding 
# destination directories in the bundle
#----------------------------------------------------------------#

	src_dirs=( "../../samples/basics/anim-basic" )
	dest_dirs=(	"lua" )
	
#----------------------------------------------------------------#
# working directory in bundle, and space-delimited list of lua 
# files to run when the application runs
#----------------------------------------------------------------#

	working_dir="lua"
	run=( "main.lua" )
	