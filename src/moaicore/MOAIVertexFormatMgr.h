// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMATMGR_H
#define	MOAIVERTEXFORMATMGR_H

#include <moaicore/MOAIVertexFormat.h>

//================================================================//
// MOAIVertexFormatMgr
//================================================================//
class MOAIVertexFormatMgr :
	public MOAIGlobalClass < MOAIVertexFormatMgr > {
public:

	enum {
		XYZC,
		XYZUVC,
		TOTAL_PRESETS,
	};

private:
	
	MOAIVertexFormat mFormats [ TOTAL_PRESETS ];
	
public:
	
	enum {
		XYZC_POSITION,
		XYZC_COLOR,
		XYZC_SIZE,
	};
	
	enum {
		XYZUVC_POSITION,
		XYZUVC_TEXCOORD,
		XYZUVC_COLOR,
		XYZUVC_SIZE,
	};
	
	//----------------------------------------------------------------//
	const MOAIVertexFormat&		GetPreset					( u32 presetID );
								MOAIVertexFormatMgr			();
								~MOAIVertexFormatMgr		();
};

#endif
