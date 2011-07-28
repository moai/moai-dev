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
		XYC,
		XYUVC,
		TOTAL_PRESETS,
	};

private:
	
	MOAIVertexFormat mFormatsFF [ TOTAL_PRESETS ];
	MOAIVertexFormat mFormatsPP [ TOTAL_PRESETS ];
	
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
		XYC_POSITION,
		XYC_COLOR,
		XYC_SIZE,
	};
	
	enum {
		XYUVC_POSITION,
		XYUVC_TEXCOORD,
		XYUVC_COLOR,
		XYUVC_SIZE,
	};
	
	//----------------------------------------------------------------//
	const MOAIVertexFormat&		GetPreset					( u32 presetID );
								MOAIVertexFormatMgr			();
								~MOAIVertexFormatMgr		();
};

#endif
