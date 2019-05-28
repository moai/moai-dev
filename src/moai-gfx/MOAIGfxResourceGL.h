// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCEGL_H
#define	MOAIGFXRESOURCEGL_H

#include <moai-gfx/MOAIGfxMgrGL.h>

//================================================================//
// MOAIAbstractGfxResourceGL
//================================================================//
/**	@lua	MOAIAbstractGfxResourceGL
	@text	Base class for graphics resources owned by OpenGL. Implements
			resource lifecycle including restoration from a lost graphics
			context (if possible).
*/
class MOAIAbstractGfxResourceGL :
	public virtual MOAIInstanceEventSource {
protected:

	friend class MOAIGfxMgr;
	friend class ZLGfxPipelineClerkGL;
	friend class ZLGfxStateGPUCacheGL;
	friend class ZLGfxResourceClerkGL;

	// for custom loading function
	MOAILuaMemberRef	mReloader;

	//----------------------------------------------------------------//
	static int		_getAge						( lua_State* L );
	static int		_getResourceState			( lua_State* L );
	static int		_purge						( lua_State* L );
	static int		_scheduleForGPUCreate		( lua_State* L );
	static int		_setReloader				( lua_State* L );

	//----------------------------------------------------------------//
	virtual ZLAbstractGfxResourceGL&		MOAIAbstractGfxResource_ZLAbstractGfxResource		() = 0;

public:

	//----------------------------------------------------------------//
								MOAIAbstractGfxResourceGL	();
	virtual						~MOAIAbstractGfxResourceGL	();
	void						RegisterLuaClass			( MOAILuaState& state );
	void						RegisterLuaFuncs			( MOAILuaState& state );
	ZLAbstractGfxResourceGL&	ZLGfxResourceGL				();
};

//================================================================//
// MOAIGfxResourceGL
//================================================================//
template < typename ZL_RESOURCE_TYPE >
class MOAIGfxResourceGL :
	public virtual MOAIAbstractGfxResourceGL,
	public virtual ZL_RESOURCE_TYPE {
protected:

	//----------------------------------------------------------------//
	MOAIGfxResourceGL () {
		this->SetGfxMgr ( MOAIGfxMgrGL::Get ());
		
		RTTI_BEGIN
			this->template ExtendRTTI < MOAIAbstractGfxResourceGL >( this );
			this->template ExtendRTTI < ZL_RESOURCE_TYPE >( this );
		RTTI_END
	}

	//----------------------------------------------------------------//
	~MOAIGfxResourceGL () {
	}

	//----------------------------------------------------------------//
	void RegisterLuaClass ( MOAILuaState& state ) {
		MOAIAbstractGfxResourceGL::RegisterLuaClass ( state );
	}
	
	//----------------------------------------------------------------//
	void RegisterLuaFuncs ( MOAILuaState& state ) {
		MOAIAbstractGfxResourceGL::RegisterLuaFuncs ( state );
	}

	//----------------------------------------------------------------//
	// virtual

	//----------------------------------------------------------------//
	ZLAbstractGfxResourceGL& MOAIAbstractGfxResource_ZLAbstractGfxResource () {
		return *this;
	}
	
	//----------------------------------------------------------------//
	void OnGfxEvent ( u32 event, void* userdata ) {
	
		// let Lua know the resource is ready for use
		this->InvokeListener ( ZLAbstractGfxResourceGL::GFX_EVENT_CREATED );
		ZL_RESOURCE_TYPE::OnGfxEvent ( event, userdata );
	}

	//----------------------------------------------------------------//
	bool ZLAbstractGfxResource_HasLoader () {
		return ( bool )this->mReloader;
	}
	
	//----------------------------------------------------------------//
	bool ZLAbstractGfxResource_InvokeLoader () {
		if ( this->mReloader ) {
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			if ( this->mReloader.PushRef ( state )) {
				state.DebugCall ( 0, 0 );
				return true;
			}
		}
		return false;
	}
};

#endif
