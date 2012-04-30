#!/bin/bash

#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

# Remove ALL files that are not under source control.
cd moai-sdk
git clean -f -d -x -e bin/
cd ..