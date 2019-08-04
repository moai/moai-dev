#!/bin/bash

#================================================================#
# Copyright (c) 2010-2017 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

spirv_header () {
    local NAME="$1"
    local INFILE="$2"
    local OUTFILE="$3"

    glslc $INFILE -o temp.spv
    echo "static cc8 $NAME [] = {" > $OUTFILE

    #TODO: some bullshit, but hexdump poops out garbage formats if input too short.
    #hexdump -v -e '"    " 16/1 "0x%02x, " "\n"' temp.spv >> $OUTFILE

    # just output one long, tall column for now. better than having to manually
    # trim the output, waste more time trying to figure hexdump out, or trying
    # to cobble together some daisychain of command line tools.
    hexdump -v -e '"    " 1/1 "0x%02x, " "\n"' temp.spv >> $OUTFILE
    echo "};" >> $OUTFILE
    rm temp.spv
}

spirv_header _deck2DShaderFSH ./glsl/MOAIDeck2DShaderVK.frag MOAIDeck2DShaderVK.frag.spv.h
spirv_header _deck2DShaderVSH ./glsl/MOAIDeck2DShaderVK.vert MOAIDeck2DShaderVK.vert.spv.h

spirv_header _oneTriShaderFSH ./glsl/MOAIOneTriShaderVK.frag MOAIOneTriShaderVK.frag.spv.h
spirv_header _oneTriShaderVSH ./glsl/MOAIOneTriShaderVK.vert MOAIOneTriShaderVK.vert.spv.h
