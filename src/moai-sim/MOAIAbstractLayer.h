// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTLAYER_H
#define	MOAIABSTRACTLAYER_H

class MOAIColor;
class MOAIFrameBufferGL;
class MOAIPartition;
class MOAIFrameBufferGL;

//================================================================//
// MOAIAbstractLayer
//================================================================//
class MOAIAbstractLayer :
	public virtual MOAILuaObject,
	public virtual MOAIAbstractDrawable {
private:

	u32				mClearFlags;
	u32				mClearColor;
	u32				mClearMode;

	MOAILuaSharedPtr < MOAIColor >			mClearColorNode;
	MOAILuaSharedPtr < MOAIFrameBufferGL >	mFrameBuffer;

	//----------------------------------------------------------------//
	static int		_draw					( lua_State* L );
	static int		_getClearMode			( lua_State* L );
	static int		_getFrameBuffer			( lua_State* L );
	static int		_pushRenderPass			( lua_State* L );
	static int		_setClearColor			( lua_State* L );
	static int		_setClearDepth			( lua_State* L );
	static int		_setClearMode			( lua_State* L );
	static int		_setFrameBuffer			( lua_State* L );

protected:

	//----------------------------------------------------------------//
	void			ClearSurface			();

	//----------------------------------------------------------------//
	void			MOAIAbstractDrawable_DrawDebug		( int subPrimID );

public:

	GET_SET ( u32, ClearFlags, mClearFlags );
	GET_SET ( u32, ClearMode, mClearMode );

	enum {
		CLEAR_NEVER,
		CLEAR_ALWAYS,
		CLEAR_ON_BUFFER_FLAG,
	};
	
	//----------------------------------------------------------------//
	MOAIFrameBufferGL*		GetFrameBuffer			();
						MOAIAbstractLayer		();
						~MOAIAbstractLayer		();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SetClearColor			( MOAIColor* color );
	void				SetFrameBuffer			( MOAIFrameBufferGL* frameBuffer = 0 );
};

#endif
