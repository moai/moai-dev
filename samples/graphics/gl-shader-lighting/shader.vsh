// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

attribute vec4 position;
attribute vec3 normal;
attribute vec2 uv;
attribute vec4 color;

varying vec4 colorVarying;
varying vec2 uvVarying;

uniform mat4 transform;
uniform mat3 normalTransform;

void main () {
    gl_Position = position * transform;
	uvVarying = uv;
	//colorVarying = color;
	
	vec3 tNormal = normalize ( normalTransform * normal );
    colorVarying = mix ( color, vec4 ( 1.0,1.0,1.0,1.0 ), max ( dot ( tNormal, vec3 ( 0.0,0.0,-1.0 )), 0.0 ));
}
