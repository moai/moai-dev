// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILINESHADER_VSH_H
#define	MOAILINESHADER_VSH_H

#define SHADER(str) #str

static cc8* _lineShaderVSH = SHADER (

	in vec4 position;
	in vec4 color;

	out LOWP vec4 colorVarying;

	void main () {
		gl_Position = position;
		colorVarying = color;
	}
);

#endif
