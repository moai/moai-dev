// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMATMGRGL_H
#define	MOAIVERTEXFORMATMGRGL_H

#include <moai-gfx/MOAIVertexFormatGL.h>

//================================================================//
// MOAIVertexFormatMgrGL
//================================================================//
class MOAIVertexFormatMgrGL :
	public ZLContextClass < MOAIVertexFormatMgrGL >,
	public virtual MOAILuaObject {
public:

	enum Preset {
		XYZC,
		XYZWC,
		XYZWUVC,
		XYZWNNNC,
		XYZWNNNUVC,
		TOTAL_FORMATS,
	};

	static const u32 UNKNOWN_FORMAT = 0xffffffff;

private:
	
	MOAIVertexFormatGL* mFormats [ TOTAL_FORMATS ];
	
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
	
	DECL_LUA_SINGLETON ( MOAIVertexFormatMgrGL )
	
	//----------------------------------------------------------------//
	MOAIVertexFormatGL*			GetFormat					( u32 formatID );
	MOAIVertexFormatGL* 		GetFormat 					( lua_State* L, int idx );
	u32							GetVertexSize				( u32 formatID );
								MOAIVertexFormatMgrGL		();
								~MOAIVertexFormatMgrGL		();
	void						RegisterLuaClass			( MOAILuaState& state );
	void						RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
