// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCEGL_H
#define	MOAIGFXRESOURCEGL_H

#include <moai-gfx-gl/MOAIGfxMgrGL_DisplayListClerkGL.h>

class MOAIGfxMgrGL;

//================================================================//
// MOAIGfxResourceGL
//================================================================//
/**	@lua	MOAIGfxResourceGL
	@text	Base class for graphics resources owned by OpenGL. Implements
			resource lifecycle including restoration from a lost graphics
			context (if possible).
*/
class MOAIGfxResourceGL :
	public virtual MOAIGfxResource,
	public virtual MOAIInstanceEventSource,
	public virtual ZLGfxListener {
public:

	enum PipelineHint {
		LOADER_PIPELINE,
		RENDER_PIPELINE,
	};

protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	friend class MOAIGfxMgrGL_DisplayListClerkGL;
	friend class MOAIGfxMgrGL_GPUCacheGL;
	friend class MOAIGfxMgrGL_ResourceClerkGL;
	
	MOAIGfxMgrGL*		mGfxMgr;
	
	u32					mState;
	u32					mLastRenderCount;

	ZLLeanLink < MOAIGfxResourceGL* > mMasterLink;
	ZLLeanLink < MOAIGfxResourceGL* > mPendingLink;

	//----------------------------------------------------------------//
	static int			_destroy					( lua_State* L );
	static int			_getAge						( lua_State* L );
	static int			_getResourceState			( lua_State* L );
	static int			_purge						( lua_State* L );
	static int			_scheduleForGPUUpdate		( lua_State* L );

	//----------------------------------------------------------------//	
	bool				Affirm						();
	bool				Bind						();
	bool				DoGPUCreate					(); // gets ready to bind
	bool				DoGPUUpdate					();
	void				Renew						(); // lose (but not *delete*) the GPU resource
	void				Unbind						();
	
	//----------------------------------------------------------------//
	void				_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	bool				MOAIGfxResource_IsReadyForUse				() const;
	void				MOAIGfxResource_ScheduleForGPUDestroy		();
	bool				MOAIGfxResource_ScheduleForGPUUpdate		();
	virtual void		MOAIGfxResourceGL_OnGPUBind					() = 0; // select GPU-side resource on device for use
	virtual bool		MOAIGfxResourceGL_OnGPUCreate				() = 0; // create GPU-side resource
	virtual void		MOAIGfxResourceGL_OnGPUDeleteOrDiscard		( bool shouldDelete ) = 0; // delete or discard GPU resource handles
	virtual void		MOAIGfxResourceGL_OnGPUUnbind				() = 0; // unbind GPU-side resource
	virtual bool		MOAIGfxResourceGL_OnGPUUpdate				() = 0;
	void				ZLGfxListener_OnGfxEvent					( u32 event, void* userdata );

public:

	enum {
		GFX_EVENT_CREATED,
		GFX_EVENT_TOTAL,
	};

	enum {
		STATE_UNINITIALIZED,			// we use this state to ensure we call DoCPUAffirm after init
		STATE_READY_FOR_GPU_CREATE,
		STATE_PENDING,					// waiting for GPU
		STATE_READY_TO_BIND,
		STATE_NEEDS_GPU_UPDATE,
		STATE_ERROR,
	};

	GET ( u32, LastRenderCount, mLastRenderCount )
	GET ( u32, State, mState )
	IS ( Pending, mState, STATE_PENDING )

	//----------------------------------------------------------------//
					MOAIGfxResourceGL			();
	virtual			~MOAIGfxResourceGL			();
	bool			Purge						( u32 age );
	bool			ScheduleForGPUUpdate		( PipelineHint hint = RENDER_PIPELINE );
};

#endif
