// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIDECK2DTEXONLYSHADER_FSH_H
#define	MOAIDECK2DTEXONLYSHADER_FSH_H

#define SHADER(str) #str

static cc8* _deck2DTexOnlyShaderFSH = SHADER (

	varying MEDP vec2 uvVarying;
	
	uniform sampler2D sampler;

	void main () {
		gl_FragColor = texture2D ( sampler, uvVarying );
	}
);

#endif
