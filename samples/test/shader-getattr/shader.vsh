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

	float xScale = 1.0 + ( xWarp * position.y * position.y );
	float yScale = 1.0 + ( yWarp * position.x * position.x );

    vec4 p = position;
	p.x *= xScale;
	p.y *= yScale;
	
	gl_Position = p; 
	uvVarying = uv;
    colorVarying = color;
}
