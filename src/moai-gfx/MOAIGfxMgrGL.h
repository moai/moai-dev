// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_H
#define	MOAIGFXMGRGL_H

#include <moai-gfx/MOAIAbstractGfxMgr.h>

//================================================================//
// MOAIGfxMgrGL
//================================================================//
// TODO: doxygen
class MOAIGfxMgrGL :
	public ZLContextClass < MOAIGfxMgrGL >,
	public virtual MOAIAbstractGfxMgr,
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
	
	//----------------------------------------------------------------//
	ZLAbstractShader*		MOAIAbstractGfxMgr_AffirmShader		( MOAILuaState& state, int idx ) const;
	ZLAbstractTexture*		MOAIAbstractGfxMgr_AffirmTexture	( MOAILuaState& state, int idx ) const;
	
public:

	DECL_LUA_SINGLETON ( MOAIGfxMgrGL )

	//----------------------------------------------------------------//
							MOAIGfxMgrGL			();
							~MOAIGfxMgrGL			();
	void					RegisterLuaClass		( MOAILuaState& state );
	
	//----------------------------------------------------------------//
	static MOAIGfxMgrGL& Get () {
		return ZLContextClass < MOAIGfxMgrGL >::Get ();
	}
};

#endif
