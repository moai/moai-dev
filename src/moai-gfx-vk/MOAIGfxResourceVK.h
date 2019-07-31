// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCEVK_H
#define	MOAIGFXRESOURCEVK_H

class MOAIGfxMgrVK;

//================================================================//
// MOAIGfxResourceVK
//================================================================//
/**	@lua	MOAIGfxResourceVK
	@text	Base class for graphics resources owned by OpenVK. Implements
			resource lifecycle including restoration from a lost graphics
			context (if possible).
*/
class MOAIGfxResourceVK :
	public virtual MOAIGfxResource,
	public virtual MOAIInstanceEventSource {
protected:

//	friend class MOAIGfxMgrVK_PipelineClerkVK;
//	friend class MOAIGfxMgrVK_GPUCacheVK;
//	friend class MOAIGfxMgrVK_ResourceClerkVK;
//
//	MOAIGfxMgrVK*		mGfxMgr;
//
//	u32					mState;
//	u32					mLastRenderCount;
//
//	ZLLeanLink < MOAIGfxResourceVK* > mMasterLink;
//	ZLLeanLink < MOAIGfxResourceVK* > mPendingLink;

	//----------------------------------------------------------------//
//	static int		_getAge						( lua_State* L );
//	static int		_getResourceState			( lua_State* L );
//	static int		_purge						( lua_State* L );

	//----------------------------------------------------------------//	
//	bool			Affirm						();
//	u32				Bind						(); // bind OR create
//	bool			DoGPUCreate					(); // gets ready to bind
//	bool			DoGPUUpdate					();
//
//	void			Renew						(); // lose (but not *delete*) the GPU resource
//	void			Unbind						();
	
	//----------------------------------------------------------------//
	bool			MOAIGfxResource_DoCPUCreate					();
	bool			MOAIGfxResource_IsReady						() const;
	void			MOAIGfxResource_ScheduleForGPUDestroy		();
	bool			MOAIGfxResource_ScheduleForGPUUpdate		( PipelineHint hint );
	virtual void	MOAIGfxResourceVK_OnGPUBind					() = 0; // select GPU-side resource on device for use
	virtual bool	MOAIGfxResourceVK_OnGPUCreate				() = 0; // create GPU-side resource
	virtual void	MOAIGfxResourceVK_OnGPUDeleteOrDiscard		( bool shouldDelete ) = 0; // delete or discard GPU resource handles
	virtual void	MOAIGfxResourceVK_OnGPUUnbind				() = 0; // unbind GPU-side resource
	virtual bool	MOAIGfxResourceVK_OnGPUUpdate				() = 0;
	void			MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
//	void			ZLGfxListener_OnGfxEvent					( u32 event, void* userdata );

public:

//	enum {
//		GFX_EVENT_CREATED,
//		GFX_EVENT_TOTAL,
//	};
//
//	enum {
//		STATE_UNINITIALIZED,			// we use this state to ensure we call DoCPUAffirm after init
//		STATE_READY_FOR_CPU_CREATE,
//		STATE_READY_FOR_GPU_CREATE,
//		STATE_PENDING,					// waiting for GPU
//		STATE_READY_TO_BIND,
//		STATE_NEEDS_GPU_UPDATE,
//		STATE_ERROR,
//	};
//
//	GET ( u32, LastRenderCount, mLastRenderCount )
//	GET ( u32, State, mState )
//	IS ( Pending, mState, STATE_PENDING )
//	IS ( Ready, mState, STATE_READY_TO_BIND )

	//----------------------------------------------------------------//
					MOAIGfxResourceVK				();
	virtual			~MOAIGfxResourceVK				();
//	bool			Purge							( u32 age );
};

#endif
