// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFERVK_H
#define	MOAIGFXBUFFERVK_H

#include <moai-gfx-vk/MOAIGfxResourceVK.h>

class MOAIVertexFormatVK;

//================================================================//
// MOAIGfxBufferVK
//================================================================//
/**	@lua	MOAIGfxBufferVK
	@text	Base class for MOAIVertexBufferVK and MOAIIndexBufferVK.
*/
class MOAIGfxBufferVK :
	public virtual MOAIGfxResourceVK,
	public virtual MOAIGfxBuffer {
protected:
	
//	friend class MOAIGfxMgrVK_PipelineClerkVK;
//	friend class MOAIGfxMgrVK_GPUCacheVK;
//	friend class MOAIVertexArrayItem;
//	friend class MOAIVertexArrayVK;
//
//	enum {
//		UPDATE_MODE_MAPBUFFER,
//		UPDATE_MODE_ORPHAN,
//		UPDATE_MODE_SUBDATA,
//	};
//
//	ZLLeanArray < ZLGfxHandle >		mVBOs;
//	ZLIndex							mCurrentVBO;
//	u32								mTarget;
//
//	bool							mUseVBOs;
//	bool							mCopyOnUpdate;
	
	//----------------------------------------------------------------//
//	static int				_reserveVBOs			( lua_State* L );
	
	//----------------------------------------------------------------//
//	ZLSharedConstBuffer*	GetBufferForBind		( ZLGfx& gfx );
	
	//----------------------------------------------------------------//
	bool					MOAIGfxResource_OnCPUCreate					();
	void 					MOAIGfxResource_OnCPUDestroy				();
	void					MOAIGfxResource_OnCPUPurgeRecoverable		();
	void					MOAIGfxResourceVK_OnGPUBind					();
	bool					MOAIGfxResourceVK_OnGPUCreate				();
	void					MOAIGfxResourceVK_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void					MOAIGfxResourceVK_OnGPUUnbind				();
	bool					MOAIGfxResourceVK_OnGPUUpdate				();
	void					MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_SerializeIn					( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void					MOAILuaObject_SerializeOut					( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );

public:
	
//	GET ( size_t, BufferCount, mVBOs.Size ())
//	GET ( u32, Target, mTarget )
//	GET_SET ( bool, CopyOnUpdate, mCopyOnUpdate )
//
//	IS ( UsingVBOs, mUseVBOs, true )
	
	//----------------------------------------------------------------//
//	size_t					CountVBOs					();
							MOAIGfxBufferVK				();
							~MOAIGfxBufferVK			();
//	void					ReserveVBOs					( ZLSize gpuBuffers );
};

#endif
