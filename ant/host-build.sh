#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

set -e

# check for command line switches
usage="usage: $0 [-r localRootFolder]"

while [ $# -gt 0 ];	do
    case "$1" in
		--debug)  debug="$2"; shift;;
		--clean)  clean="$2"; shift;;
		-*)
	    	echo >&2 \
	    		$usage
	    	exit 1;;
		*)  break;;	# terminate while loop
    esac
    shift
done

echo "Building: debug: "${debug}" clean: "$clean

if [ "$debug" == "true" ]; then
	build_cmd="ant debug"
else
	build_cmd="ant release"
fi

pushd host/project > /dev/null
	if [ "$clean" == "all" ] || [ "$clean" == "app" ]; then
		ant clean
	fi
	$build_cmd
popd > /dev/null
