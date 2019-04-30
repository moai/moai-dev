// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTGFXRESOURCE_H
#define	ZLABSTRACTGFXRESOURCE_H

#include <zl-gfx/ZLGfxListener.h>

class ZLGfx;
class ZLGfxMgrGL;

//================================================================//
// ZLAbstractGfxResource
//================================================================//
/**	@lua	ZLAbstractGfxResource
	@text	Base class for graphics resources owned by OpenGL. Implements
			resource lifecycle including restoration from a lost graphics
			context (if possible).
*/
class ZLAbstractGfxResource :
	public virtual RTTIBase,
	public ZLGfxListener {
protected:

	friend class ZLGfxPipelineClerk;
	friend class ZLGfxStateGPUCache;
	friend class ZLGfxResourceClerk;
	
	ZLGfxMgrGL*			mGfxMgr;
	
	u32					mState;
	u32					mLastRenderCount;

	ZLLeanLink < ZLAbstractGfxResource* > mMasterLink;
	ZLLeanLink < ZLAbstractGfxResource* > mPendingLink;

	//----------------------------------------------------------------//	
	bool			Affirm						();
	u32				Bind						(); // bind OR create
	bool			DoGPUCreate					(); // gets ready to bind
	bool			DoGPUUpdate					();
	bool			HasLoader					();
	void			Renew						(); // lose (but not *delete*) the GPU resource
	void			Unbind						();
	
	//----------------------------------------------------------------//
	virtual bool	ZLAbstractGfxResource_HasLoader					();
	virtual bool	ZLAbstractGfxResource_InvokeLoader				();
	virtual void	ZLAbstractGfxResource_OnClearDirty				();
	virtual bool	ZLAbstractGfxResource_OnCPUCreate				() = 0; // load or initialize any CPU-side resources required to create the GPU-side resource
	virtual void	ZLAbstractGfxResource_OnCPUDestroy				() = 0; // clear any CPU-side memory used by class
	virtual void	ZLAbstractGfxResource_OnGPUBind					() = 0; // select GPU-side resource on device for use
	virtual bool	ZLAbstractGfxResource_OnGPUCreate				() = 0; // create GPU-side resource
	virtual void	ZLAbstractGfxResource_OnGPUDeleteOrDiscard		( bool shouldDelete ) = 0; // delete or discard GPU resource handles
	virtual void	ZLAbstractGfxResource_OnGPUUnbind				() = 0; // unbind GPU-side resource
	virtual bool	ZLAbstractGfxResource_OnGPUUpdate				() = 0;
	void			ZLGfxListener_OnGfxEvent						( u32 event, void* userdata );

public:

	enum {
		GFX_EVENT_CREATED,
		GFX_EVENT_TOTAL,
	};

	enum {
		STATE_UNINITIALIZED,			// we use this state to ensure we call DoCPUAffirm after init
		STATE_READY_FOR_CPU_CREATE,
		STATE_READY_FOR_GPU_CREATE,
		STATE_PENDING,					// waiting for GPU
		STATE_READY_TO_BIND,
		STATE_NEEDS_GPU_UPDATE,
		STATE_ERROR,
	};

	GET ( u32, LastRenderCount, mLastRenderCount )
	GET ( u32, State, mState )
	IS ( Pending, mState, STATE_PENDING )
	IS ( Ready, mState, STATE_READY_TO_BIND )

	//----------------------------------------------------------------//
	void			Destroy						(); // delete CPU and GPU data; go back to STATE_NEW
	bool			DoCPUCreate					(); // preload CPU portion
	void			FinishInit					(); // ready to CPU/GPU affirm; recover from STATE_NEW or STATE_ERROR
	bool			InvokeLoader				();
	bool			Purge						( u32 age );
	bool			ScheduleForGPUCreate		( u32 pipelineID );
	bool			ScheduleForGPUUpdate		();
	void			SetGfxMgr					( ZLGfxMgrGL& gfxMgr );
					ZLAbstractGfxResource		();
	virtual			~ZLAbstractGfxResource		();
};

#endif
