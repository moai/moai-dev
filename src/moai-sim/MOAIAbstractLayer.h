// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTLAYER_H
#define	MOAIABSTRACTLAYER_H

class MOAIColor;
class MOAIFrameBuffer;
class MOAIPartition;
class MOAIFrameBuffer;

//================================================================//
// MOAIAbstractLayer
//================================================================//
class MOAIAbstractLayer :
	public virtual MOAILuaObject,
	public virtual MOAIAbstractDrawable {
private:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	u32				mClearFlags;
	u32				mClearColor;
	u32				mClearMode;

	MOAILuaSharedPtr < MOAIColor >			mClearColorNode;
	MOAILuaSharedPtr < MOAIFrameBuffer >	mFrameBuffer;

	//----------------------------------------------------------------//
	static int		_draw						( lua_State* L );
	static int		_getClearMode				( lua_State* L );
	static int		_getFrameBuffer				( lua_State* L );
	static int		_pushRenderPass				( lua_State* L );
	static int		_setClearColor				( lua_State* L );
	static int		_setClearDepth				( lua_State* L );
	static int		_setClearMode				( lua_State* L );
	static int		_setFrameBuffer				( lua_State* L );

protected:

	//----------------------------------------------------------------//
	void			ClearSurface				();

	//----------------------------------------------------------------//
	void			_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIDrawable_DrawDebug		( int subPrimID );

public:

	GET_SET ( u32, ClearFlags, mClearFlags );
	GET_SET ( u32, ClearMode, mClearMode );

	enum {
		CLEAR_NEVER,
		CLEAR_ALWAYS,
		CLEAR_ON_BUFFER_FLAG,
	};
	
	//----------------------------------------------------------------//
	MOAIFrameBuffer*	GetFrameBuffer			();
						MOAIAbstractLayer		();
						~MOAIAbstractLayer		();
	void				SetClearColor			( MOAIColor* color );
	void				SetFrameBuffer			( MOAIFrameBuffer* frameBuffer = 0 );
};

#endif
