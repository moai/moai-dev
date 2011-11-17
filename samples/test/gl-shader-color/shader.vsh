// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

attribute vec4 position;
attribute vec2 uv;
attribute vec4 color;

varying vec4 colorVarying;
varying vec2 uvVarying;

void main () {
    gl_Position = position; 
	uvVarying = uv;
    colorVarying = color;
}
