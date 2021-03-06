// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONTSNAPPINGSHADER_FSH_H
#define	MOAIFONTSNAPPINGSHADER_FSH_H

#define SHADER(str) #str

static cc8* _fontSnappingShaderFSH = SHADER (

	varying LOWP vec4 colorVarying;
	varying MEDP vec2 uvVarying;
	
	uniform sampler2D sampler;

	void main () {
		gl_FragColor = colorVarying * texture2D ( sampler, uvVarying )[ 3 ];
	}
);

#endif
