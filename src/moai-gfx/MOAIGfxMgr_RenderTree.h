// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGR_RENDERTREE_H
#define	MOAIGFXMGR_RENDERTREE_H

//================================================================//
// MOAIGfxMgr_RenderTree
//================================================================//
// TODO: doxygen
class MOAIGfxMgr_RenderTree {
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
	virtual void	MOAIGfxMgr_RenderTree_Render		() = 0;
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );

public:
	
	GET ( u32, RenderCounter, mRenderCounter )
	GET ( double, RenderDuration, mRenderDuration )
	GET ( double, RenderTime, mRenderTime )
	
	//----------------------------------------------------------------//
					MOAIGfxMgr_RenderTree		();
	virtual			~MOAIGfxMgr_RenderTree		();
	void			PushDrawable				( MOAILuaObject* drawable );
	void			Render						();
};

#endif
