// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILINESHADER_VSH_H
#define	MOAILINESHADER_VSH_H

#define SHADER(str) #str

static cc8* _lineShaderVSH = SHADER (

	attribute vec4 position;
	attribute vec4 color;

	varying LOWP vec4 colorVarying;

	void main () {
		gl_Position = position;
		colorVarying = color;
	}
);

#endif
