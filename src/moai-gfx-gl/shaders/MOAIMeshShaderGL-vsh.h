// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHSHADER_VSH_H
#define	MOAIMESHSHADER_VSH_H

#define SHADER(str) #str

static cc8* _meshShaderVSH = SHADER (
	
	in vec4 position;
	in vec2 uv;
	in vec4 color;

	uniform mat4 transform;
	uniform vec4 ucolor;

	out MEDP vec2 uvVarying;
	out LOWP vec4 colorVarying;

	void main () {
		gl_Position = transform * position;
		uvVarying = uv;
		colorVarying = color * ucolor;
	}
);

#endif
