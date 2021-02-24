// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK2DSHADER_VSH_H
#define	MOAIDECK2DSHADER_VSH_H

#define SHADER(str) #str

static cc8* _deck2DShaderVSH = SHADER (

	in vec4 position;
	in vec2 uv;
	in vec4 color;

	out MEDP vec2 uvVarying;
	out LOWP vec4 colorVarying;

	void main () {
		gl_Position = position;
		uvVarying = uv;
		colorVarying = color;
	}
);

#endif
