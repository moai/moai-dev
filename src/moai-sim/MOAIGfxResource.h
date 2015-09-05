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
	public ZLGfxListener {
private:

	friend class MOAIGfxDevice;
	friend class MOAIGfxDeviceBase;
	friend class MOAIGfxDeviceStateCache;
	friend class MOAIGfxResourceMgr;

	u32					mState;
	bool				mScheduled;
	u32					mLastRenderCount;

	ZLLeanLink < MOAIGfxResource* > mLink;

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
	bool			Bind						(); // bind for drawing; go to STATE_READY
	bool			DoGPUCreate					(); // gets ready to bind
	bool			InvokeLoader				();
	void			Renew						(); // lose (but not *delete*) the GPU resource
	void			Unbind						();

protected:

	enum {
		GFX_EVENT_CREATED,
	};

	//----------------------------------------------------------------//
	void			FinishInit					(); // ready to CPU/GPU affirm; recover from STATE_NEW or STATE_ERROR
	bool			HasReloader					();
	virtual bool	OnCPUCreate					() = 0; // load or initialize any CPU-side resources required to create the GPU-side resource
	virtual void	OnCPUDestroy				() = 0; // clear any CPU-side memory used by class
	
	void			OnGfxEvent					( u32 event, void* userdata );
	
	virtual void	OnGPUBind					() = 0; // select GPU-side resource on device for use
	virtual bool	OnGPUCreate					() = 0; // create GPU-side resource
	virtual void	OnGPUDestroy				() = 0; // schedule GPU-side resource for destruction
	virtual void	OnGPULost					() = 0; // clear any handles or references to GPU-side (called by 'Abandon')
	virtual void	OnGPUUnbind					() = 0; // unbind GPU-side resource

public:

	enum {
		STATE_UNINITIALIZED, // we use this state to ensure we call DoCPUAffirm after init
		STATE_READY_FOR_CPU_CREATE,
		STATE_READY_FOR_GPU_CREATE,
		STATE_READY_TO_BIND,
		STATE_ERROR,
	};

	GET ( u32, State, mState )
	IS ( Ready, mState, STATE_READY_TO_BIND )
	IS ( Scheduled, mScheduled, true )

	//----------------------------------------------------------------//
	bool			Affirm						();
	void			Destroy						(); // delete CPU and GPU data; go back to STATE_NEW
	bool			DoCPUCreate					(); // preload CPU portion
					MOAIGfxResource				();
	virtual			~MOAIGfxResource			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	bool			ScheduleForGPUCreate		( u32 listID );
	bool			Purge						( u32 age );
};

#endif
