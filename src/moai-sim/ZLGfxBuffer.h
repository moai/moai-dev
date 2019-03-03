// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLGFXBUFFER_H
#define	ZLGFXBUFFER_H

#include <moai-sim/MOAIGfxResource.h>

class MOAIVertexFormat;

//================================================================//
// ZLGfxBuffer
//================================================================//
/**	@lua	ZLGfxBuffer
	@text	Base class for MOAIVertexBuffer and MOAIIndexBuffer.
*/
class ZLGfxBuffer :
	public ZLAbstractGfxResource,
	public virtual ZLStream,
	public ZLCopyOnWrite {
protected:
	
	friend class ZLGfxPipelineClerk;
	friend class ZLGfxStateGPUCache;
	friend class MOAIVertexArrayItem;
	friend class ZLVertexArray;
	
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
	ZLSharedConstBuffer*	GetBufferForBind		( ZLGfx& gfx );
	
	//----------------------------------------------------------------//
	bool					ZLAbstractGfxResource_OnCPUCreate				();
	void					ZLAbstractGfxResource_OnCPUDestroy				();
	void					ZLAbstractGfxResource_OnGPUBind					();
	bool					ZLAbstractGfxResource_OnGPUCreate				();
	void					ZLAbstractGfxResource_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void					ZLAbstractGfxResource_OnGPUUnbind				();
	bool					ZLAbstractGfxResource_OnGPUUpdate				();

public:
	
	GET ( size_t, BufferCount, mVBOs.Size ())
	GET ( u32, Target, mTarget )
	GET_SET ( bool, CopyOnUpdate, mCopyOnUpdate )
	
	IS ( UsingVBOs, mUseVBOs, true )
	
	//----------------------------------------------------------------//
	void						Clear					();
	void						CopyFromStream			( ZLStream& stream, size_t size );
	size_t						CountVBOs				();
//	ZLSharedConstBuffer*		GetBuffer				();
	void						Reserve					( ZLSize size );
	void						ReserveVBOs				( ZLSize gpuBuffers );
								ZLGfxBuffer				();
								~ZLGfxBuffer			();
};

#endif
