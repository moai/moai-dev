// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONTSNAPPINGSHADER_VSH_H
#define	MOAIFONTSNAPPINGSHADER_VSH_H

#define SHADER(str) #str

static cc8* _fontSnappingShaderVSH = SHADER (

	attribute vec4 position;
	attribute vec2 uv;
	attribute vec4 color;

	uniform float xSnap;
	uniform float ySnap;

	varying LOWP vec4 colorVarying;
	varying MEDP vec2 uvVarying;

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
