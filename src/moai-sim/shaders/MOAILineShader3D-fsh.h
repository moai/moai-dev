// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILINESHADER3D_FSH_H
#define	MOAILINESHADER3D_FSH_H

#define SHADER(str) #str

static cc8* _lineShader3DFSH = SHADER (

	varying LOWP vec4 colorVarying;

	void main() {
		gl_FragColor = colorVarying;
	}
);

#endif
