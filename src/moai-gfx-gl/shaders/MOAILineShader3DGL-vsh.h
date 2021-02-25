// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILINESHADER3D_VSH_H
#define	MOAILINESHADER3D_VSH_H

#define SHADER(str) #str

static cc8* _lineShader3DVSH = SHADER (

	in vec4 position;
	in vec4 color;

	uniform mat4 transform;
	uniform vec4 ucolor;

	out LOWP vec4 colorVarying;

	void main () {
		gl_Position = transform * position;
		colorVarying = color * ucolor;
	}
);

#endif
