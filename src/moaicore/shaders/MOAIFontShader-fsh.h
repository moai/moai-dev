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
		gl_FragColor.xyz = colorVarying.xyz;
		gl_FragColor.a = colorVarying.a * texture2D ( sampler, uvVarying ).a;
	}
);

#endif
