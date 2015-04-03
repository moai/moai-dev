#!/bin/sh
#--------------------------------------------------------------------------------------
# Copyright (c) 2010-2013 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#--------------------------------------------------------------------------------------

cd `dirname $0`

# Verify paths
if [ ! -f "$MOAI_BIN/moai" ]; then
    echo "---------------------------------------------------------------------------"
    echo "Error: The MOAI_BIN environment variable doesn't exist or its pointing to an"
    echo "invalid path.  Please point it at a folder containing moai executable"
    echo "---------------------------------------------------------------------------"
    exit 1
fi

# Run moai
$MOAI_BIN/moai main.lua
