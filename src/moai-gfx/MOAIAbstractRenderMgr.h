// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTRENDERMGR_H
#define	MOAIABSTRACTRENDERMGR_H

//================================================================//
// MOAIAbstractRenderMgr
//================================================================//
// TODO: doxygen
class MOAIAbstractRenderMgr :
	public ZLContextClassAlias < MOAIAbstractRenderMgr >,
	public virtual MOAILuaObject {
protected:

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

	//----------------------------------------------------------------//
	virtual void	MOAIAbstractRenderMgr_Render	() = 0;

public:

	DECL_LUA_SINGLETON ( MOAIAbstractRenderMgr )
	
	GET ( u32, RenderCounter, mRenderCounter )
	GET ( double, RenderDuration, mRenderDuration )
	GET ( double, RenderTime, mRenderTime )
	
	//----------------------------------------------------------------//
					MOAIAbstractRenderMgr		();
					~MOAIAbstractRenderMgr		();
	void			PushDrawable				( MOAILuaObject* drawable );
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			Render						();
};

#endif
