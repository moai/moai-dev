// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com								 
varying LOWP vec4 colorVarying;
varying MEDP vec2 uvVarying;

uniform sampler2D sampler;
uniform float outlineMin;
uniform float outlineMax;

void main () {

	vec4 clr;
	clr.rgb = colorVarying.rgb;

	float distAlphaMask = texture2D ( sampler, uvVarying )[ 3 ];
	float oFactor = 1.0;

	if ( distAlphaMask <= outlineMax && distAlphaMask >= outlineMin )
	{
		oFactor = smoothstep( outlineMax, outlineMin, distAlphaMask );
		clr = mix ( clr, vec4(1, 1, 1, 1), oFactor);
		//clr.rgb = vec3(1, 1, 1);
	}

	if ( distAlphaMask >= 0.50 )
		clr.a = 1.0;
	else
		clr.a = 0.0;

	//clr.a *= smoothstep(0.25, 0.75, texture2D ( sampler, uvVarying )[ 3 ]);

	gl_FragColor = clr;

	//gl_FragColor [ 0 ] = colorVarying [ 0 ];
	//gl_FragColor [ 1 ] = colorVarying [ 1 ];
	//gl_FragColor [ 2 ] = colorVarying [ 2 ];
	//gl_FragColor [ 3 ] = colorVarying [ 3 ] * texture2D ( sampler, uvVarying )[ 3 ];
}
