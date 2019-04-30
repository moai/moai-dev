// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_H
#define	MOAIGFXMGRGL_H

#include <moai-gfx/MOAIGfxMgr.h>

//================================================================//
// MOAIGfxMgrGL
//================================================================//
// TODO: doxygen
class MOAIGfxMgrGL :
	public ZLContextClass < MOAIGfxMgrGL >,
	public virtual MOAIGfxMgr,
	public virtual ZLGfxMgrGL {
private:
	
	//----------------------------------------------------------------//
	static int			_enablePipelineLogging		( lua_State* L );
	static int			_getFrameBuffer				( lua_State* L );
	static int			_getMaxTextureSize			( lua_State* L );
	static int			_getMaxTextureUnits			( lua_State* L );
	static int			_getViewSize				( lua_State* L );
	static int			_purgeResources				( lua_State* L );
	static int			_renewResources				( lua_State* L );
	
	//----------------------------------------------------------------//
	void				OnGlobalsFinalize			();
	void				OnGlobalsInitialize			();
	
public:

	DECL_LUA_SINGLETON ( MOAIGfxMgrGL )

	//----------------------------------------------------------------//
						MOAIGfxMgrGL				();
						~MOAIGfxMgrGL				();
	void				RegisterLuaClass			( MOAILuaState& state );
	
	//----------------------------------------------------------------//
	static MOAIGfxMgrGL& Get () {
		return ZLContextClass < MOAIGfxMgrGL >::Get ();
	}
};

#endif
