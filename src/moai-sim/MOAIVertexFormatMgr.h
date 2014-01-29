// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMATMGR_H
#define	MOAIVERTEXFORMATMGR_H

#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIVertexFormatMgr
//================================================================//
class MOAIVertexFormatMgr :
	public MOAIGlobalClass < MOAIVertexFormatMgr, MOAILuaObject > {
public:

	enum {
		XYZC,
		XYZWC,
		XYZWUVC,
		TOTAL_PRESETS,
	};

private:
	
	MOAIVertexFormat* mFormats [ TOTAL_PRESETS ];
	
public:
	
	enum {
		XYZC_POSITION,
		XYZC_COLOR,
	};
	
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
	
	DECL_LUA_SINGLETON ( MOAIVertexFormatMgr )
	
	//----------------------------------------------------------------//
	MOAIVertexFormat&		GetPreset					( u32 presetID );
	u32						GetVertexSize				( u32 presetID );
							MOAIVertexFormatMgr			();
							~MOAIVertexFormatMgr		();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
