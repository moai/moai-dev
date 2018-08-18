// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	VIDEOSHADERYCRCBITUREC601FULLRANGEBIPLANAR_VSH_H
#define	VIDEOSHADERYCRCBITUREC601FULLRANGEBIPLANAR_VSH_H

#define SHADER(str) #str

static cc8* _videoShaderYCrCbITURec601FullRangeBiPlanarVSH = SHADER (

	attribute vec4 position;
	attribute vec2 uv;

	varying LOWP vec4 colorVarying;
	varying MEDP vec2 uvVarying;

	void main () {
		gl_Position = position;
		uvVarying = uv;
	}
);

#endif
