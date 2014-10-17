// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONTSHADER_FSH_H
#define	MOAIFONTSHADER_FSH_H

#define SHADER(str) #str

static cc8* _fontShaderFSH = SHADER (

	varying LOWP vec4 colorVarying;
	varying MEDP vec2 uvVarying;
	
	uniform sampler2D sampler;

	void main () {
		gl_FragColor [ 0 ] = colorVarying [ 0 ];
		gl_FragColor [ 1 ] = colorVarying [ 1 ];
		gl_FragColor [ 2 ] = colorVarying [ 2 ];
		gl_FragColor [ 3 ] = colorVarying [ 3 ] * texture2D ( sampler, uvVarying )[ 3 ];
	}
);

#endif
