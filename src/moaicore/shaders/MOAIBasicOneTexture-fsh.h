// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBASICONETEXTURE_FSH_H
#define	MOAIBASICONETEXTURE_FSH_H

#define SHADER(str) #str

static cc8* _basicOneTextureFSH = SHADER (

	varying vec4 colorVarying;
	varying vec2 uvVarying;
	
	uniform sampler2D sampler;

	void main() {
		gl_FragColor = texture2D ( sampler, uvVarying ) * colorVarying;
	}
);

#endif
