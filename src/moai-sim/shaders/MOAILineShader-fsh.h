// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILINESHADER_FSH_H
#define	MOAILINESHADER_FSH_H

#define SHADER(str) #str

static cc8* _lineShaderFSH = SHADER (

	varying LOWP vec4 colorVarying;

	void main () {
		gl_FragColor = colorVarying;
	}
);

#endif
