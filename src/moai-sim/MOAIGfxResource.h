// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCE_H
#define	MOAIGFXRESOURCE_H

//================================================================//
// MOAIGfxResource
//================================================================//
/**	@lua	MOAIGfxResource
	@text	Base class for graphics resources owned by OpenGL. Implements
			resource lifecycle including restoration from a lost graphics
			context (if possible).
*/
class MOAIGfxResource :
	public virtual MOAIInstanceEventSource,
	public virtual ZLGfxListener {
private:

	friend class MOAIGfxMgr;
	friend class MOAIGfxPipelineClerk;
	friend class MOAIGfxStateCache;
	friend class MOAIGfxResourceClerk;

	u32					mState;
	u32					mLastRenderCount;

	ZLLeanLink < MOAIGfxResource* > mMasterLink;
	ZLLeanLink < MOAIGfxResource* > mPendingLink;

	// for custom loading function
	MOAILuaMemberRef	mReloader;

	//----------------------------------------------------------------//
	static int		_getAge						( lua_State* L );
	static int		_getResourceState			( lua_State* L );
	static int		_preload					( lua_State* L );
	static int		_purge						( lua_State* L );
	static int		_scheduleForGPUCreate		( lua_State* L );
	static int		_setReloader				( lua_State* L );

	//----------------------------------------------------------------//
	void			Affirm						();
	u32				Bind						(); // bind OR create
	bool			DoGPUCreate					(); // gets ready to bind
	bool			DoGPUUpdate					();
	bool			InvokeLoader				();
	void			Renew						(); // lose (but not *delete*) the GPU resource
	void			Unbind						();

protected:

	enum {
		GFX_EVENT_CREATED,
		GFX_EVENT_TOTAL,
	};

	//----------------------------------------------------------------//
	void			FinishInit					(); // ready to CPU/GPU affirm; recover from STATE_NEW or STATE_ERROR
	bool			HasReloader					();
	virtual void	OnClearDirty				();
	virtual bool	OnCPUCreate					() = 0; // load or initialize any CPU-side resources required to create the GPU-side resource
	virtual void	OnCPUDestroy				() = 0; // clear any CPU-side memory used by class
	void			OnGfxEvent					( u32 event, void* userdata );
	virtual void	OnGPUBind					() = 0; // select GPU-side resource on device for use
	virtual bool	OnGPUCreate					() = 0; // create GPU-side resource
	virtual void	OnGPUDeleteOrDiscard		( bool shouldDelete ) = 0; // delete or discard GPU resource handles
	virtual void	OnGPUUnbind					() = 0; // unbind GPU-side resource
	virtual bool	OnGPUUpdate					() = 0;

public:

	enum {
		STATE_UNINITIALIZED,			// we use this state to ensure we call DoCPUAffirm after init
		STATE_READY_FOR_CPU_CREATE,
		STATE_READY_FOR_GPU_CREATE,
		STATE_PENDING,					// waiting for GPU
		STATE_READY_TO_BIND,
		STATE_NEEDS_GPU_UPDATE,
		STATE_ERROR,
	};

	GET ( u32, State, mState )
	IS ( Pending, mState, STATE_PENDING )
	IS ( Ready, mState, STATE_READY_TO_BIND )

	//----------------------------------------------------------------//
	void			Destroy						(); // delete CPU and GPU data; go back to STATE_NEW
	bool			DoCPUCreate					(); // preload CPU portion
					MOAIGfxResource				();
	virtual			~MOAIGfxResource			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	bool			ScheduleForGPUCreate		( u32 pipelineID );
	bool			ScheduleForGPUUpdate		();
	bool			Purge						( u32 age );
};

#endif
