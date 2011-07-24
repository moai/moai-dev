----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

uniform mat4 transform;

attribute vec4 position;
attribute vec4 color;

varying vec4 colorVarying;

void main () {
    gl_Position = position * transform;
    colorVarying = color;
}
