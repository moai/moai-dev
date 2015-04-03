// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCE_H
#define	MOAIGFXRESOURCE_H

//================================================================//
// MOAIGfxState
//================================================================//
/**	@lua	MOAIGfxState
	@text	Abstract base class for objects that represent changes
			to graphics state.
*/
class MOAIGfxState :
	public virtual MOAILuaObject {
private:

	friend class MOAIGfxDevice;

	//----------------------------------------------------------------//
	// this is for binding via the gfx device's cache; we need this since
	// Bind () is supposed to be ignorant of
	virtual bool		LoadGfxState			() { return false; }

public:

	//----------------------------------------------------------------//
						MOAIGfxState			();
	virtual				~MOAIGfxState			();
};

//================================================================//
// MOAIGfxResource
//================================================================//
/**	@lua	MOAIGfxResource
	@text	Base class for graphics resources owned by OpenGL. Implements
			resource lifecycle including restoration from a lost graphics
			context (if possible).
*/
class MOAIGfxResource :
	public MOAIGfxState {
private:

	enum {
		STATE_NEW, // we use this state to ensure we call DoCPUAffirm after init
		STATE_NEEDS_CPU_CREATE,
		STATE_NEEDS_GPU_CREATE,
		STATE_READY_TO_BIND,
		STATE_ERROR,
	};

	u32					mState;
	u32					mLastRenderCount;
	u32					mLoadingPolicy;

	ZLLeanLink < MOAIGfxResource* > mLink;

	// for custom loading function
	MOAILuaMemberRef	mReloader;

	//----------------------------------------------------------------//
	static int		_getAge						( lua_State* L );
	static int		_purge						( lua_State* L );
	static int		_setLoadingPolicy			( lua_State* L );
	static int		_setReloader				( lua_State* L );

	//----------------------------------------------------------------//
	bool			DoGPUAffirm					(); // gets ready to bind
	void			InvokeLoader				();
	void			Renew						(); // lose (but not *delete*) the GPU resource

protected:

	//----------------------------------------------------------------//
	void			FinishInit					(); // ready to CPU/GPU affirm; recover from STATE_NEW or STATE_ERROR
	bool			HasReloader					();
	
	virtual bool	OnCPUCreate					() = 0; // load or initialize any CPU-side resources required to create the GPU-side resource
	virtual void	OnCPUDestroy				() = 0; // clear any CPU-side memory used by class
	virtual void	OnGPUBind					() = 0; // select GPU-side resource on device for use
	virtual bool	OnGPUCreate					() = 0; // create GPU-side resource
	virtual void	OnGPUDestroy				() = 0; // schedule GPU-side resource for destruction
	virtual void	OnGPULost					() = 0; // clear any handles or references to GPU-side (called by 'Abandon')
	virtual void	OnGPUUnbind					() = 0; // unbind GPU-side resource

public:

	friend class MOAIGfxDevice;
	friend class MOAIGfxResourceMgr;

	GET ( u32, State, mState );
	
	enum {
		LOADING_POLICY_NONE,				// don't care and/or use global policy
		LOADING_POLICY_CPU_GPU_ASAP,		// load everything asap
		LOADING_POLICY_CPU_ASAP_GPU_NEXT,	// load the cpu part asap; load the gpu part next render
		LOADING_POLICY_CPU_ASAP_GPU_BIND,	// load the cpu part asap, load the gpu part on bind (during render)
		LOADING_POLICY_CPU_GPU_BIND,		// load the cpu and gpu part on bind (during render)
	};

	static const u32 DEFAULT_LOADING_POLICY = LOADING_POLICY_CPU_ASAP_GPU_NEXT;

	// if the build defines MOAI_USE_GFX_THREAD, then gpu loading can *only* happen during render. this only effects
	// LOADING_POLICY_CPU_GPU_ASAP, in which case the gpu portion must be added to a queue and loaded the next time
	// we render.

	//----------------------------------------------------------------//
	bool			DoCPUAffirm					(); // preload CPU portion
	bool			Bind						(); // bind for drawing; go to STATE_READY
	void			Destroy						(); // delete CPU and GPU data; go back to STATE_NEW
	void			ForceCPUCreate				();
	virtual u32		GetLoadingPolicy			();
					MOAIGfxResource				();
	virtual			~MOAIGfxResource			();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	bool			Purge						( u32 age );
	bool			PrepareForBind				();
	void			Unbind						();
};

#endif
