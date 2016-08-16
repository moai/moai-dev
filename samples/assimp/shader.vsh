// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

const int MAX_BONES = 100;

attribute vec4		position;
attribute vec3		normal;
attribute vec2		uv;
attribute vec4		color;

attribute vec4		boneIndices;
attribute vec4		boneWeights;

varying vec4 colorVarying;
varying vec2 uvVarying;

uniform mat4 transforms [ 2 ];
uniform mat4 bones [ MAX_BONES ];

void main () {

	mat4 bone = bones [ int ( boneIndices.x )] * boneWeights.x;
    bone +=  bones [ int ( boneIndices.y )] * boneWeights.y;
    bone +=  bones [ int ( boneIndices.z )] * boneWeights.z;
    bone +=  bones [ int ( boneIndices.w )] * boneWeights.w;

    gl_Position = transforms [ 1 ] * transforms [ 0 ] * bone * position;

	uvVarying = uv;
	colorVarying = color;
}
