// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMATMGRGL_H
#define	MOAIVERTEXFORMATMGRGL_H

#include <moai-gfx-gl/MOAIVertexFormatGL.h>

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
	
	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
	
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
	MOAIVertexFormatGL*			GetFormat					( MOAIVertexFormatPresetEnum formatID );
	MOAIVertexFormatGL* 		GetFormat 					( lua_State* L, int idx );
	u32							GetVertexSize				( MOAIVertexFormatPresetEnum formatID );
								MOAIVertexFormatMgrGL		();
								~MOAIVertexFormatMgrGL		();
};

#endif
