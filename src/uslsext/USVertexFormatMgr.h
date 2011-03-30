// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USVERTEXFORMATMGR_H
#define	USVERTEXFORMATMGR_H

#include <uslsext/USVertexFormat.h>

//================================================================//
// USVertexFormatMgr
//================================================================//
class USVertexFormatMgr :
	public USGlobalClass < USVertexFormatMgr > {
public:

	enum {
		VTX_FMT_XYC,
		VTX_FMT_XYUVC,
		TOTAL_PRESETS,
		CUSTOM_FORMAT,
	};

private:
	
	USVertexFormat mFormats [ TOTAL_PRESETS ];
	
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
	
	//----------------------------------------------------------------//
	const USVertexFormat&	GetPreset				( u32 presetID );
							USVertexFormatMgr		();
							~USVertexFormatMgr		();
};

#endif
