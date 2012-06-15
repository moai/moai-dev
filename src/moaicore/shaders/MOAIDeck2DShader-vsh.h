// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK2DSHADER_VSH_H
#define	MOAIDECK2DSHADER_VSH_H

#define SHADER(str) #str

static cc8* _deck2DShaderVSH = SHADER (

	attribute vec4 position;
	attribute vec2 uv;
	attribute vec4 color;

	varying LOWP vec4 colorVarying;
	varying MEDP vec2 uvVarying;

	void main () {
		gl_Position = position;
		uvVarying = uv;
		colorVarying = color;
	}
);

#endif
