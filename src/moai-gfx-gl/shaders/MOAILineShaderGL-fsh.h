// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILINESHADER_FSH_H
#define	MOAILINESHADER_FSH_H

#define SHADER(str) #str

static cc8* _lineShaderFSH = SHADER (

	in LOWP vec4 colorVarying;
	
	out MEDP vec4 fragColor;
	
	void main () {
		fragColor = colorVarying;
	}
);

#endif
