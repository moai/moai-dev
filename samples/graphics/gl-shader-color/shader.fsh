// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

varying LOWP vec4 colorVarying;
varying MEDP vec2 uvVarying;

uniform sampler2D sampler;
uniform vec4 maskColor;

void main() { 
	gl_FragColor = ( texture2D ( sampler, uvVarying ) * colorVarying ) + maskColor;
}
