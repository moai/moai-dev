// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMATMGR_H
#define	MOAIVERTEXFORMATMGR_H

#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIVertexFormatMgr
//================================================================//
class MOAIVertexFormatMgr :
	public ZLContextClass < MOAIVertexFormatMgr, MOAILuaObject > {
public:

	enum {
		XYZC,
		XYZWC,
		XYZWUVC,
		XYZWNNNC,
		XYZWNNNUVC,
		TOTAL_FORMATS,
	};

	static const u32 UNKNOWN_FORMAT = 0xffffffff;

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
	
	enum {
		XYZWNNNC_POSITION,
		XYZWNNNC_NORMAL,
		XYZWNNNC_COLOR,
	};
	
	enum {
		XYZWNNNUVC_POSITION,
		XYZWNNNUVC_NORMAL,
		XYZWNNNUVC_TEXCOORD,
		XYZWNNNUVC_COLOR,
	};
	
	DECL_LUA_SINGLETON ( MOAIVertexFormatMgr )
	
	//----------------------------------------------------------------//
	MOAIVertexFormat*			GetFormat					( u32 formatID );
	u32							GetVertexSize				( u32 formatID );
								MOAIVertexFormatMgr			();
								~MOAIVertexFormatMgr		();
	void						RegisterLuaClass			( MOAILuaState& state );
	void						RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
