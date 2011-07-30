// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBASICNOTEXTURE_FSH_H
#define	MOAIBASICNOTEXTURE_FSH_H

#define SHADER(str) #str

static cc8* _basicNoTextureFSH = SHADER (

	varying LOWP vec4 colorVarying;

	void main() {
		gl_FragColor = colorVarying;
	}
);

#endif
