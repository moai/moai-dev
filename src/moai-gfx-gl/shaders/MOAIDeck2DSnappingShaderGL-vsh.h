// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK2DSNAPPINGSHADER_VSH_H
#define	MOAIDECK2DSNAPPINGSHADER_VSH_H

#define SHADER(str) #str

static cc8* _deck2DSnappingShaderVSH = SHADER (

	in vec4 position;
	in vec2 uv;
	in vec4 color;

	uniform float xSnap;
	uniform float ySnap;

	out MEDP vec2 uvVarying;
	out LOWP vec4 colorVarying;

	void main () {
		
		gl_Position.x = floor (( position.x * xSnap ) + 0.5 ) / xSnap;
		gl_Position.y = floor (( position.y * ySnap ) + 0.5 ) / ySnap;
		gl_Position.z = position.z;
		gl_Position.w = position.w;
		
		uvVarying = uv;
		colorVarying = color;
	}
);

#endif
