#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

	set -e

	usage="usage: scons.sh [-p platform] [-clean]"
	platform=pepper_17
	clean=

	# check for valid NACL SDK root
	if [ "$NACL_SDK_ROOT" = "" ] || [ ! -d $NACL_SDK_ROOT ]; then
	
		echo -e "*** Please specify a valid path to the NACL SDK in the NACL_SDK_ROOT"
		echo -e "*** environment variable before running this script."
		echo
		exit 1
	fi

	while [ $# -gt 0 ];	do
    	case "$1" in
			-p)  platform="$2"; shift;;
			-clean) clean="--clean";;
			-*)
	    		echo >&2 \
	    			$usage
	    		exit 1;;
				*)  break;;	# terminate while loop
    		esac
    		shift
		done

	export NACL_TARGET_PLATFORM=$platform
	export NACL_PLATFORM_DIR=$NACL_SDK_ROOT/$platform

	export SCONS_LIB_DIR="${NACL_PLATFORM_DIR}/third_party/scons-2.0.1/engine"
	export PYTHONPATH="${NACL_PLATFORM_DIR}/third_party/scons-2.0.1/engine"

	# We have to do this because scons overrides PYTHONPATH and does not preserve
	# what is provided by the OS.  The custom variable name won't be overwritten.
	export PYMOX="${NACL_SDK_ROOT}/third_party/pymox"

	if [ "$clean" != "" ]; then
		echo "Cleaning"
	else
		echo "Building Moai Native Client binaries for $platform"
	fi
	
	python -O -OO $NACL_PLATFORM_DIR/third_party/scons-2.0.1/script/scons \
		--warn no-visual-c-missing \
		$clean \
		--file=build.scons \
		--site-dir="${NACL_PLATFORM_DIR}/build_tools/nacl_sdk_scons" \
		$*

	if [ "$clean" != "" ]; then
		rm -rf lib*
	fi