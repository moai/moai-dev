// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

varying LOWP vec4 colorVarying;
varying MEDP vec2 uvVarying;

// TODO: expose as uniforms
vec4 outlineColor = vec4 ( 0.0, 1.0, 1.0, 1.0 );
vec4 glowColor = vec4 ( 0.0, 0.0, 1.0, 1.0 );

uniform sampler2D sampler;

void main () {

	vec4 sample = texture2D ( sampler, uvVarying );

	float sdf = sample.a;
	vec4 texColor = vec4 ( sample.r, sample.g, sample.b, 1.0 ) * colorVarying;

	if ( sdf > 0.5 ) {

		gl_FragColor = texColor;
	}
	else if ( sdf > 0.45 ) {

		gl_FragColor = mix ( outlineColor, texColor, ( sdf - 0.45 ) * 20.0 );
	}
	else if ( sdf > 0.35 ) {

		gl_FragColor = outlineColor;
	}
	else if ( sdf > 0.3 ) {

		gl_FragColor = mix ( glowColor * sdf, outlineColor, ( sdf - 0.3 ) * 20.0 );
	}
	else {

		gl_FragColor = glowColor * sdf;
	}
}
