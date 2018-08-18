// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERMGR_H
#define	MOAIRENDERMGR_H

//================================================================//
// MOAIRenderMgr
//================================================================//
// TODO: doxygen
class MOAIRenderMgr :
	public ZLContextClass < MOAIRenderMgr, MOAILuaObject > {
private:

	u32				mRenderCounter;	// increments every render
	double			mRenderDuration;
	double			mRenderTime;
	
	MOAILuaStrongRef	mRenderRoot;
	
	//----------------------------------------------------------------//
	static int		_getRenderCount				( lua_State* L );
	static int		_getRender					( lua_State* L );
	static int		_setRender					( lua_State* L );

	//----------------------------------------------------------------//
	void			RenderTable					( MOAILuaState& state, int idx );

public:

	DECL_LUA_SINGLETON ( MOAIRenderMgr )
	
	GET ( u32, RenderCounter, mRenderCounter )
	GET ( double, RenderDuration, mRenderDuration )
	GET ( double, RenderTime, mRenderTime )
	
	//----------------------------------------------------------------//
					MOAIRenderMgr				();
					~MOAIRenderMgr				();
	void			PushDrawable				( MOAILuaObject* drawable );
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			Render						();
};

#endif
