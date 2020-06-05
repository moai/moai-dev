// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERBATCHGL_H
#define	MOAIRENDERBATCHGL_H

//================================================================//
// MOAIRenderBatchGL
//================================================================//
class MOAIRenderBatchGL :
	public virtual MOAIRenderBatch {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	MOAILuaSharedPtr < MOAIFrameBuffer >	mFrameBuffer;

	//----------------------------------------------------------------//
	static int				_getFrameBuffer				( lua_State* L );
	static int				_setFrameBuffer				( lua_State* L );

	//----------------------------------------------------------------//
	void					_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	void					MOAIRenderBatch_Render		();

public:

	DECL_LUA_FACTORY ( MOAIRenderBatchGL )

	//----------------------------------------------------------------//
	MOAIFrameBuffer*		GetFrameBuffer				();
							MOAIRenderBatchGL			();
							~MOAIRenderBatchGL			();
	void					SetFrameBuffer				( MOAIFrameBuffer* frameBuffer = 0 );
};

#endif
