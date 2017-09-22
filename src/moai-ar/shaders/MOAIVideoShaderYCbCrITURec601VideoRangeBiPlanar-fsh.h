// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	VIDEOSHADERYCBCRITUREC601VIDEORANGEBIPLANAR_FSH_H
#define	VIDEOSHADERYCBCRITUREC601VIDEORANGEBIPLANAR_FSH_H

#define SHADER(str) #str

static cc8* _videoShaderYCbCrITURec601VideoRangeBiPlanarFSH = SHADER (

	varying MEDP vec2 uvVarying;
	
	uniform sampler2D texture0;
	uniform sampler2D texture1;

	void main () {
		
		vec3 colourYCbCr;
		const mat3 transformYCbCrITURec601VideoRangeToRGB = mat3 (
			1.164,		1.164,			1.164,		// Column 0
			0.0,		-0.391,			2.017,		// Column 1
			1.596,		-0.813,			0.0			// Column 2
		);
		colourYCbCr.x  = texture2D ( texture0, uvVarying ).r - 0.0625;
		colourYCbCr.yz = texture2D ( texture1, uvVarying ).ra - 0.5;
		gl_FragColor = vec4 ( transformYCbCrITURec601FullRangeToRGB * colourYCbCr, 1.0 );
	}
);

#endif
