// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

varying LOWP vec4 colorVarying;
varying MEDP vec2 uvVarying;

uniform sampler2D sdfSampler;
uniform sampler2D rampSampler;

uniform vec4 outlineColor;
uniform vec4 glowColor;

void main () {

	vec4 sdfSample = texture2D ( sdfSampler, uvVarying );

	float sdf = sdfSample.a;

	vec4 texColor = vec4 ( sdfSample.r, sdfSample.g, sdfSample.b, sdf > 0.5 ? 1.0 : 0.0 ) * colorVarying;
	
	sdf = 1.0 - sdf;

	float ramp0 = texture2D ( rampSampler, vec2 ( 0.25, sdf )).a;
	float ramp1 = texture2D ( rampSampler, vec2 ( 0.75, sdf )).a;
	
	texColor = mix ( texColor, glowColor, ramp1 );
	gl_FragColor = mix ( texColor, outlineColor, ramp0 );
}
