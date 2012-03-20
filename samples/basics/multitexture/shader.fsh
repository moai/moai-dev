// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

varying MEDP vec2 uvVarying0;
varying MEDP vec2 uvVarying1;

uniform sampler2D sampler0;
uniform sampler2D sampler1;

void main() { 
	gl_FragColor = texture2D ( sampler0, uvVarying0 ) + texture2D ( sampler1, uvVarying1 );
}
