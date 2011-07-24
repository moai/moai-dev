// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMATMGR_H
#define	MOAIVERTEXFORMATMGR_H

#include <moaicore/MOAIVertexFormat.h>

//================================================================//
// MOAIVertexFormatMgr
//================================================================//
class MOAIVertexFormatMgr :
	public USGlobalClass < MOAIVertexFormatMgr > {
public:

	enum {
		FF_XYC,
		FF_XYUVC,
		PP_XYUVC,
		TOTAL_PRESETS,
	};

private:
	
	MOAIVertexFormat mFormats [ TOTAL_PRESETS ];
	
public:
	
	//----------------------------------------------------------------//
	struct VtxXYC {
		float	mX, mY;
		u32		mC;
	};
	
	//----------------------------------------------------------------//
	struct VtxXYUVC {
		float	mX, mY;
		float	mU, mV;
		u32		mC;
	};
	
	enum {
		PP_XYUVC_POSITION,
		PP_XYUVC_TEXCOORD,
		PP_XYUVC_COLOR,
		PP_XYUVC_SIZE,
	};
	
	//----------------------------------------------------------------//
	const MOAIVertexFormat&	GetPreset				( u32 presetID );
							MOAIVertexFormatMgr		();
							~MOAIVertexFormatMgr		();
};

#endif
