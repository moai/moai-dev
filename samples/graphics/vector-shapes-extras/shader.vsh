// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

attribute vec4 position;
attribute vec4 color; // the vertex color
attribute float blend; // interpolate between color0, color and color1

uniform mat4 transform;
uniform vec4 ucolor;
uniform vec4 color0;
uniform vec4 color1;

varying LOWP vec4 colorVarying;

void main () {

	gl_Position = position * transform;
	
	vec4 blendColor = color0 + (( color1 - color0 ) * blend );
	
	colorVarying = color + (( blendColor - color ) * blendColor.w );	
	colorVarying.w = color.w;
}
