// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHSHADER_VSH_H
#define	MOAIMESHSHADER_VSH_H

#define SHADER(str) #str

static cc8* _meshShaderVSH = SHADER (

	// Default should be highp and this breaks non ES OpenGL
	//precision highp float;

	attribute vec4 position;
	attribute vec2 uv;
	attribute vec4 color;

	uniform mat4 transform;
	uniform vec4 ucolor;

	varying vec4 colorVarying;
	varying vec2 uvVarying;

	void main () {
		gl_Position = position * transform;
		uvVarying = uv;
		colorVarying = color * ucolor;
	}
);

#endif
