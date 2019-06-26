// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXFORMATMGRVK_H
#define	MOAIVERTEXFORMATMGRVK_H

#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIVertexFormatMgrVK
//================================================================//
class MOAIVertexFormatMgrVK :
	public ZLContextClass < MOAIVertexFormatMgrVK >,
	public virtual MOAILuaObject {
public:

//	enum Preset {
//		XYZC,
//		XYZWC,
//		XYZWUVC,
//		XYZWNNNC,
//		XYZWNNNUVC,
//		TOTAL_FORMATS,
//	};
//
//	static const u32 UNKNOWN_FORMAT = 0xffffffff;

private:
	
//	MOAIVertexFormatVK* mFormats [ TOTAL_FORMATS ];
	
	MOAIVertexFormatVK* mFormat;
	
	//----------------------------------------------------------------//
//	static int			_getFormat			( lua_State* L );
	
	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
	
public:
	
//	enum {
//		XYZC_POSITION,
//		XYZC_COLOR,
//	};
//
//	enum {
//		XYZWC_POSITION,
//		XYZWC_COLOR,
//	};
//
//	enum {
//		XYZWUVC_POSITION,
//		XYZWUVC_TEXCOORD,
//		XYZWUVC_COLOR,
//	};
//
//	enum {
//		XYZWNNNC_POSITION,
//		XYZWNNNC_NORMAL,
//		XYZWNNNC_COLOR,
//	};
//
//	enum {
//		XYZWNNNUVC_POSITION,
//		XYZWNNNUVC_NORMAL,
//		XYZWNNNUVC_TEXCOORD,
//		XYZWNNNUVC_COLOR,
//	};
	
	DECL_LUA_SINGLETON ( MOAIVertexFormatMgrVK )
	
	//----------------------------------------------------------------//
	MOAIVertexFormatVK*			GetFormat					( MOAIVertexFormatPresetEnum formatID );
//	MOAIVertexFormatVK* 		GetFormat 					( lua_State* L, int idx );
//	u32							GetVertexSize				( MOAIVertexFormatPresetEnum formatID );
								MOAIVertexFormatMgrVK		();
								~MOAIVertexFormatMgrVK		();
};

#endif
