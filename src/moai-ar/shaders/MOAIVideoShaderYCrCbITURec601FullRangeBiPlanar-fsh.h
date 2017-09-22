// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	VIDEOSHADERYCRCBITUREC601FULLRANGEBIPLANAR_FSH_H
#define	VIDEOSHADERYCRCBITUREC601FULLRANGEBIPLANAR_FSH_H

#define SHADER(str) #str

static cc8* _videoShaderYCrCbITURec601FullRangeBiPlanarFSH = SHADER (

	varying MEDP vec2 uvVarying;
	
	uniform sampler2D texture0;
	uniform sampler2D texture1;

	void main () {
		
		vec3 colourYCrCb;
		const mat3 transformYCrCbITURec601FullRangeToRGB = mat3 (
			1.0,		1.0,			1.0,		// Column 0
			1.402,		-0.714,			0.0,		// Column 1
			0.0,		-0.344,			1.772		// Column 2
		);
		colourYCrCb.x  = texture2D ( texture0, uvVarying ).r;
		colourYCrCb.yz = texture2D ( texture1, uvVarying ).ra - 0.5;
		gl_FragColor = vec4 ( transformYCrCbITURec601FullRangeToRGB * colourYCrCb, 1.0 );
	}
);

#endif
