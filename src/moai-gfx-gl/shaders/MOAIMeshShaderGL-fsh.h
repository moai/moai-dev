// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHSHADER_FSH_H
#define	MOAIMESHSHADER_FSH_H

#define SHADER(str) #str

static cc8* _meshShaderFSH = SHADER (
	
	in LOWP vec4 colorVarying;
	in MEDP vec2 uvVarying;
	
	out MEDP vec4 fragColor;
	
	uniform MEDP sampler2D sampler;

	void main () {
		fragColor = texture ( sampler, uvVarying ) * colorVarying;
	}
);

#endif
