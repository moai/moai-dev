// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGR_RENDERTREE_H
#define	MOAIGFXMGR_RENDERTREE_H

class MOAIRenderBatch;

//================================================================//
// MOAIGfxMgr_RenderTree
//================================================================//
// TODO: doxygen
class MOAIGfxMgr_RenderTree {
protected:

	u32				mRenderCounter;	// increments every render
	double			mRenderDuration;
	double			mRenderTime;
	
	ZLStrongPtr < MOAIRenderBatch >		mRenderBatch; // this is the default batch
	MOAILuaStrongRef					mRenderRoot; // this is for user-configured batches
	
	//----------------------------------------------------------------//
	static int		_getRenderCount				( lua_State* L );
	static int		_getRender					( lua_State* L );
	static int		_setRender					( lua_State* L );

	//----------------------------------------------------------------//
	void			AffirmDefaultBatch			();
	void			PushDrawable				( MOAILuaStrongRef& renderRoot, MOAILuaObject* drawable );
	void			RenderTable					( MOAILuaState& state, int idx );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual void	MOAIGfxMgr_RenderTree_Render	() = 0;

public:
	
	GET ( u32, RenderCounter, mRenderCounter )
	GET ( double, RenderDuration, mRenderDuration )
	GET ( double, RenderTime, mRenderTime )
	
	//----------------------------------------------------------------//
					MOAIGfxMgr_RenderTree		();
	virtual			~MOAIGfxMgr_RenderTree		();
	void			PushDrawable				( MOAILuaObject* drawable ); // goes into the default batch, if it exists
	void			Render						();
};

#endif
