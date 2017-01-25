// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERMGR_H
#define	MOAIRENDERMGR_H

#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAIImage.h>

class MOAIRenderPass;

//================================================================//
// MOAIRenderMgr
//================================================================//
/**	@lua	MOAIRenderMgr
	@text	MOAIRenderMgr is responsible for drawing a list of MOAIDrawable
			objects. MOAIDrawable is the base class for any object that can be
			drawn. This includes MOAIProp and MOAIPartitionLayer. To use MOAIRenderMgr
			pass a table of MOAIDrawable objects to MOAIRenderMgr.setRenderTable ().
			The table will usually be a stack of MOAIPartitionLayer objects. The contents of
			the table will be rendered the next time a frame is drawn. Note that the
			table must be an array starting with index 1. Objects will be rendered
			counting from the base index until 'nil' is encountered. The render
			table may include other tables as entries. These must also be arrays
			indexed from 1.
*/
class MOAIRenderMgr :
	public ZLContextClass < MOAIRenderMgr, MOAILuaObject > {
private:

	u32				mLastDrawCount; // draw count for last frame.
	u32				mRenderCounter;	// increments every render
	double			mRenderDuration;
	double			mRenderTime;
	
	MOAILuaStrongRef	mRenderRoot;
	
	//----------------------------------------------------------------//
	static int		_getPerformanceDrawCount    ( lua_State* L );
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
	
	//GET_SET ( MOAIFrameBuffer*, FrameBuffer, mFrameBuffer )
	//GET_SET ( MOAIDrawable*, Drawable, mDrawable )
	
	//----------------------------------------------------------------//
					MOAIRenderMgr				();
					~MOAIRenderMgr				();
	void			PushDrawable				( MOAILuaObject* drawable );
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			Render						();
};

#endif
