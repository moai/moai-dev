// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK2DTEXONLYSHADER_VSH_H
#define	MOAIDECK2DTEXONLYSHADER_VSH_H

#define SHADER(str) #str

static cc8* _deck2DTexOnlyShaderVSH = SHADER (

	attribute vec4 position;
	attribute vec2 uv;
	attribute vec4 color;

	varying MEDP vec2 uvVarying;

	void main () {
		gl_Position = position;
		uvVarying = uv;
	}
);

#endif
