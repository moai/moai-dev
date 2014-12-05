// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

attribute vec4 position;
attribute vec2 uv;
attribute vec4 color;

varying vec4 colorVarying;
varying vec2 uvVarying;

uniform float xWarp;
uniform float yWarp;

void main () {

	gl_Position = position;

	float xScale = 1.0 + ( xWarp * position.y * position.y );
	float yScale = 1.0 + ( yWarp * position.x * position.x );

	gl_Position.x = position.x * xScale;
	gl_Position.y = position.y * yScale;
	
	uvVarying = uv;
    colorVarying = color;
}
