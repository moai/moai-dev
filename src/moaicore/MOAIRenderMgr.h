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
	
	u32				mLastDrawCount; // Draw count for last frame.

	u32				mRenderCounter;	// increments every render
	MOAILuaRef		mRenderTable;
	
	//----------------------------------------------------------------//
	static int		_getPerformanceDrawCount    ( lua_State* L );
	static int		_getRenderTable				( lua_State* L );
	static int		_setRenderTable				( lua_State* L );

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
