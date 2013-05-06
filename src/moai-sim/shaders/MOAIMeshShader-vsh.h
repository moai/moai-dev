// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHSHADER_VSH_H
#define	MOAIMESHSHADER_VSH_H

#define SHADER(str) #str

static cc8* _meshShaderVSH = SHADER (

	attribute vec4 position;
	attribute vec2 uv;
	attribute vec4 color;

	uniform mat4 transform;
	uniform vec4 ucolor;

	varying LOWP vec4 colorVarying;
	varying MEDP vec2 uvVarying;

	void main () {
		gl_Position = position * transform;
		uvVarying = uv;
		colorVarying = color * ucolor;
	}
);

#endif
