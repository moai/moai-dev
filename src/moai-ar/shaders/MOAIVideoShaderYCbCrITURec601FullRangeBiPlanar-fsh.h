// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	VIDEOSHADERYCBCRITUREC601FULLRANGEBIPLANAR_FSH_H
#define	VIDEOSHADERYCBCRITUREC601FULLRANGEBIPLANAR_FSH_H

#define SHADER(str) #str

static cc8* _videoShaderYCbCrITURec601FullRangeBiPlanarFSH = SHADER (

	varying LOWP vec4 colorVarying;
	varying MEDP vec2 uvVarying;
	
	uniform sampler2D texture0;
	uniform sampler2D texture1;

	void main () {
		
		vec3 colourYCbCr;
		const mat3 transformYCbCrITURec601FullRangeToRGB = mat3 (
			1.0,		1.0,			1.0,		// Column 0
			0.0,		-0.344,			1.772,		// Column 1
			1.402,		-0.714,			0.0			// Column 2
		);
		colourYCbCr.x  = texture2D ( texture0, uvVarying ).r;
		colourYCbCr.yz = texture2D ( texture1, uvVarying ).ra - 0.5;
		gl_FragColor = vec4 ( transformYCbCrITURec601FullRangeToRGB * colourYCbCr, 1.0 ) * colorVarying;
	}
);

#endif
