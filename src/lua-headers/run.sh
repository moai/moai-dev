#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e

	# check for valid Moai executable
	if [ x"$MOAI_BIN" = x"" ] || [ ! -f $MOAI_BIN/moai ]; then
		echo
		echo -e "--------------------------------------------------------------------------------"
		echo -e "ERROR: The MOAI_BIN environment variable either doesn't exist or it's pointing"
		echo -e "to an invalid path. Please point it at a folder containing moai."
		echo -e "--------------------------------------------------------------------------------"
		echo
		exit 1
	fi

	# check for valid Moai config file
	if [ x"$MOAI_CONFIG" = x"" ] || [ ! -f $MOAI_CONFIG/config.lua ]; then
		echo
		echo -e "--------------------------------------------------------------------------------"
		echo -e "ERROR: The MOAI_CONFIG environment variable either doesn't exist or it's"
		echo -e "pointing to an invalid path. Please point it at a folder containing config.lua."
		echo -e "--------------------------------------------------------------------------------"
		echo
		exit 1
	fi

	$MOAI_BIN/moai $MOAI_CONFIG/config.lua main.lua