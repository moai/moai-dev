// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	VIDEOSHADERYCBCRITUREC601VIDEORANGEBIPLANAR_VSH_H
#define	VIDEOSHADERYCBCRITUREC601VIDEORANGEBIPLANAR_VSH_H

#define SHADER(str) #str

static cc8* _videoShaderYCbCrITURec601VideoRangeBiPlanarVSH = SHADER (

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
