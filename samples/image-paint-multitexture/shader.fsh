// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

varying MEDP vec2 uvVarying;

uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform sampler2D sampler2;

void main() {
	float a = texture2D ( sampler0, uvVarying ).a;
	gl_FragColor = ( texture2D ( sampler1, uvVarying ) * a ) + ( texture2D ( sampler2, uvVarying ) * ( 1.0 - a ));
}
