// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

attribute vec4 position;
attribute vec3 normal;
attribute vec2 uv;
attribute vec4 color;

varying vec4 colorVarying;
varying vec2 uvVarying;

uniform mat4 worldViewProj;
uniform mat3 worldNorm;

void main () {

    gl_Position = worldViewProj * position;
	uvVarying = uv;
	
	vec3 tNormal = normalize ( worldNorm * normal );
	vec3 lightDirection = normalize ( vec3 ( -0.5, -1.0, -1.0 ));
    colorVarying = vec4 ( color.xyz * max ( 0.0, dot ( tNormal, -lightDirection )), color.a );
    //colorVarying = vec4 ( tNormal, 1.0 );
    //colorVarying = color;
}
