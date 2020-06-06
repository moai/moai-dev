// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERBATCHVK_H
#define	MOAIRENDERBATCHVK_H

class MOAIFrameBufferVK;
class MOAIRenderPassVK;

//================================================================//
// MOAIRenderBatchVK
//================================================================//
class MOAIRenderBatchVK :
	public virtual MOAIRenderBatch {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

//	ZLStrongPtr < MOAIAbstractDrawable >	mOneTri;

	MOAILuaSharedPtr < MOAIFrameBufferVK >	mFrameBuffer;
	MOAILuaSharedPtr < MOAIRenderPassVK >	mRenderPassVK;

	//----------------------------------------------------------------//
	static int				_getFrameBuffer				( lua_State* L );
	static int				_setFrameBuffer				( lua_State* L );

	//----------------------------------------------------------------//
	void					_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	void					MOAIRenderBatch_Render		();

public:

	DECL_LUA_FACTORY ( MOAIRenderBatchVK )

	//----------------------------------------------------------------//
	MOAIFrameBufferVK*		GetFrameBuffer				();
	MOAIRenderPassVK*		GetRenderPass				();
							MOAIRenderBatchVK			();
							~MOAIRenderBatchVK			();
	void					SetFrameBuffer				( MOAIFrameBufferVK* frameBuffer = 0 );
};

#endif
