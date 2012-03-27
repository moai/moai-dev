// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERMGR_H
#define	MOAIRENDERMGR_H

#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAILua.h>

class MOAIProp;

//================================================================//
// MOAIRenderMgr
//================================================================//
/**	@name	MOAIRenderMgr
	@text	Manages render table.
*/
class MOAIRenderMgr :
	public MOAIGlobalClass < MOAIRenderMgr, MOAILuaObject > {
private:
	
	u32				mRenderCounter;	// increments every render
	MOAILuaRef		mRenderTable;
	
	//----------------------------------------------------------------//
	static int		_getRenderTable				( lua_State* L );
	static int		_setRenderTable				( lua_State* L );

	//----------------------------------------------------------------//
	#ifdef DOXYGEN
		static int		_clearRenderStack		( lua_State* L );
		static int		_popRenderPass			( lua_State* L );
		static int		_pushRenderPass			( lua_State* L );
		static int		_removeRenderPass		( lua_State* L );
	#endif

	//----------------------------------------------------------------//
	void			RenderTable					( MOAILuaState& state, int idx );

public:

	DECL_LUA_SINGLETON ( MOAIRenderMgr )
	
	GET ( u32, RenderCounter, mRenderCounter )
	
	//----------------------------------------------------------------//
					MOAIRenderMgr				();
					~MOAIRenderMgr				();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			Render						();
};

#endif
