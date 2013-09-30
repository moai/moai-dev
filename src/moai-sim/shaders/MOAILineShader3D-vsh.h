// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILINESHADER3D_VSH_H
#define	MOAILINESHADER3D_VSH_H

#define SHADER(str) #str

static cc8* _lineShader3DVSH = SHADER (

	attribute vec4 position;
	attribute vec4 color;

	uniform mat4 transform;
	uniform vec4 ucolor;

	varying LOWP vec4 colorVarying;

	void main () {
		gl_Position = position * transform;
		colorVarying = color * ucolor;
	}
);

#endif
