// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESHSHADER_FSH_H
#define	MOAIMESHSHADER_FSH_H

#define SHADER(str) #str

static cc8* _meshShaderFSH = SHADER (

	varying LOWP vec4 colorVarying;
	varying MEDP vec2 uvVarying;
	
	uniform sampler2D sampler;

	void main() {
		gl_FragColor = texture2D ( sampler, uvVarying ) * colorVarying;
	}
);

#endif
