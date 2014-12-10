// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com								 
varying LOWP vec4 colorVarying;
varying MEDP vec2 uvVarying;

uniform sampler2D sampler;
uniform float outlineMin;
uniform float outlineMax;
uniform vec4 outlineColor;

void main () {

	float distAlphaMask = texture2D ( sampler, uvVarying )[ 3 ];
	float oFactor = 1.0;

	vec4 clr;
	clr.rgb = colorVarying.rgb;

	if ( distAlphaMask >= 0.50 )
		clr.a = 1.0;
	else
		clr.a = 0.0;

	if ( distAlphaMask <= outlineMax && distAlphaMask >= outlineMin )
	{
		clr.a = 1.0;
		oFactor = smoothstep( outlineMin, outlineMax, distAlphaMask );
		clr = mix ( clr, outlineColor, oFactor);
		//clr.rgb = vec3(1, 1, 1);
	}

	gl_FragColor = clr;

	//gl_FragColor [ 0 ] = colorVarying [ 0 ];
	//gl_FragColor [ 1 ] = colorVarying [ 1 ];
	//gl_FragColor [ 2 ] = colorVarying [ 2 ];
	//gl_FragColor [ 3 ] = colorVarying [ 3 ] * texture2D ( sampler, uvVarying )[ 3 ];
}
