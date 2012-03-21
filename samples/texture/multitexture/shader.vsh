// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

attribute vec4 position;
attribute vec2 uv;
attribute vec4 color;

varying vec2 uvVarying0;
varying vec2 uvVarying1;

void main () {
    gl_Position = position; 
	uvVarying0 = uv;
	uvVarying1 = position * vec2 ( 0.5, -0.5 );
}
