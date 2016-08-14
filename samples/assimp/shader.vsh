// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

const int MAX_BONES = 100;

attribute vec4 position;
attribute vec3 normal;
attribute vec2 uv;
attribute vec4 color;

varying vec4 colorVarying;
varying vec2 uvVarying;

uniform mat4 world;
uniform mat4 viewProj;
uniform mat4 bones [ MAX_BONES ];

void main () {

    gl_Position = viewProj * world * position;
	uvVarying = uv;
	colorVarying = color;
}
