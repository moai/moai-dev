#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

set -e

# Remove ALL files that are not under source control, except binaries.
pushd moai-sdk > /dev/null
	git clean -f -d -x -e bin/
popd > /dev/null