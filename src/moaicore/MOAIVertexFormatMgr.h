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
		XYZWC,
		XYZWUVC,
		TOTAL_PRESETS,
	};

private:
	
	MOAIVertexFormat mFormats [ TOTAL_PRESETS ];
	
public:
	
	enum {
		XYZWC_POSITION,
		XYZWC_COLOR,
		XYZWC_SIZE,
	};
	
	enum {
		XYZWUVC_POSITION,
		XYZWUVC_TEXCOORD,
		XYZWUVC_COLOR,
		XYZWUVC_SIZE,
	};
	
	//----------------------------------------------------------------//
	const MOAIVertexFormat&		GetPreset					( u32 presetID );
								MOAIVertexFormatMgr			();
								~MOAIVertexFormatMgr		();
};

#endif
