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
		TOTAL_FORMATS,
	};

private:
	
	MOAIVertexFormat* mFormats [ TOTAL_FORMATS ];
	
	//----------------------------------------------------------------//
	static int			_getFormat			( lua_State* L );
	
public:
	
	enum {
		XYZC_POSITION,
		XYZC_COLOR,
	};
	
	enum {
		XYZWC_POSITION,
		XYZWC_COLOR,
	};
	
	enum {
		XYZWUVC_POSITION,
		XYZWUVC_TEXCOORD,
		XYZWUVC_COLOR,
	};
	
	DECL_LUA_SINGLETON ( MOAIVertexFormatMgr )
	
	//----------------------------------------------------------------//
	MOAIVertexFormat&		GetFormat					( u32 formatID );
	u32						GetVertexSize				( u32 formatID );
							MOAIVertexFormatMgr			();
							~MOAIVertexFormatMgr		();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
