// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXBUFFERGL_H
#define	MOAIGFXBUFFERGL_H

#include <moai-gfx-gl/MOAIGfxResourceGL.h>

class MOAIVertexFormatGL;

//================================================================//
// MOAIGfxBufferGL
//================================================================//
/**	@lua	MOAIGfxBufferGL
	@text	Base class for MOAIVertexBufferGL and MOAIIndexBufferGL.
*/
class MOAIGfxBufferGL :
	public virtual MOAIGfxResourceGL,
	public virtual MOAIGfxBuffer {
protected:
	
	friend class MOAIGfxMgrGL_PipelineClerkGL;
	friend class MOAIGfxMgrGL_GPUCacheGL;
	friend class MOAIVertexArrayItem;
	friend class MOAIVertexArrayGL;
	
	enum {
		UPDATE_MODE_MAPBUFFER,
		UPDATE_MODE_ORPHAN,
		UPDATE_MODE_SUBDATA,
	};
	
	ZLLeanArray < ZLGfxHandle >		mVBOs;
	ZLIndex							mCurrentVBO;
	u32								mTarget;

	bool							mUseVBOs;
	bool							mCopyOnUpdate;
	
	//----------------------------------------------------------------//
	static int				_reserveVBOs			( lua_State* L );
	
	//----------------------------------------------------------------//
	ZLSharedConstBuffer*	GetBufferForBind		( ZLGfx& gfx );
	
	//----------------------------------------------------------------//
	bool					MOAIGfxResource_OnCPUCreate					();
	void 					MOAIGfxResource_OnCPUDestroy				();
	void					MOAIGfxResource_OnCPUPurgeRecoverable		();
	void					MOAIGfxResourceGL_OnGPUBind					();
	bool					MOAIGfxResourceGL_OnGPUCreate				();
	void					MOAIGfxResourceGL_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void					MOAIGfxResourceGL_OnGPUUnbind				();
	bool					MOAIGfxResourceGL_OnGPUUpdate				();

public:
	
	GET ( size_t, BufferCount, mVBOs.Size ())
	GET ( u32, Target, mTarget )
	GET_SET ( bool, CopyOnUpdate, mCopyOnUpdate )
	
	IS ( UsingVBOs, mUseVBOs, true )
	
	//----------------------------------------------------------------//
	size_t					CountVBOs					();
							MOAIGfxBufferGL				();
							~MOAIGfxBufferGL			();
	void					MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );
	void					ReserveVBOs					( ZLSize gpuBuffers );
	void					MOAILuaObject_SerializeIn					( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void					MOAILuaObject_SerializeOut				( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
};

#endif
