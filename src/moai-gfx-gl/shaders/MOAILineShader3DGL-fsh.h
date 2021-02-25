// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILINESHADER3D_FSH_H
#define	MOAILINESHADER3D_FSH_H

#define SHADER(str) #str

static cc8* _lineShader3DFSH = SHADER (

	in LOWP vec4 colorVarying;
	
	out MEDP vec4 fragColor;
	
	void main () {
		fragColor = colorVarying;
	}
);

#endif
