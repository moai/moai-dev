// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXRESOURCE_H
#define	MOAIGFXRESOURCE_H

#include <moai-sim/ZLAbstractGfxResource.h>

//================================================================//
// MOAIAbstractGfxResource
//================================================================//
/**	@lua	MOAIAbstractGfxResource
	@text	Base class for graphics resources owned by OpenGL. Implements
			resource lifecycle including restoration from a lost graphics
			context (if possible).
*/
class MOAIAbstractGfxResource :
	public virtual MOAIInstanceEventSource {
protected:

	friend class MOAIGfxMgr;
	friend class MOAIGfxPipelineClerk;
	friend class ZLGfxStateGPUCache;
	friend class ZLGfxResourceClerk;

	// for custom loading function
	MOAILuaMemberRef	mReloader;

	//----------------------------------------------------------------//
	static int		_getAge						( lua_State* L );
	static int		_getResourceState			( lua_State* L );
	static int		_purge						( lua_State* L );
	static int		_scheduleForGPUCreate		( lua_State* L );
	static int		_setReloader				( lua_State* L );

	//----------------------------------------------------------------//
	virtual ZLAbstractGfxResource&		MOAIAbstractGfxResource_ZLAbstractGfxResource		() = 0;

public:

	//----------------------------------------------------------------//
								MOAIAbstractGfxResource		();
	virtual						~MOAIAbstractGfxResource	();
	void						RegisterLuaClass			( MOAILuaState& state );
	void						RegisterLuaFuncs			( MOAILuaState& state );
	ZLAbstractGfxResource&		ZLGfxResource				();
};

//================================================================//
// MOAIGfxResource
//================================================================//
template < typename ZL_RESOURCE_TYPE >
class MOAIGfxResource :
	public virtual MOAIAbstractGfxResource,
	public virtual ZL_RESOURCE_TYPE {
protected:

	//----------------------------------------------------------------//
	ZLAbstractGfxResource& MOAIAbstractGfxResource_ZLAbstractGfxResource () {
		return *this;
	}
	
	//----------------------------------------------------------------//
	void RegisterLuaClass ( MOAILuaState& state ) {
		MOAIAbstractGfxResource::RegisterLuaClass ( state );
	}
	
	//----------------------------------------------------------------//
	void RegisterLuaFuncs ( MOAILuaState& state ) {
		MOAIAbstractGfxResource::RegisterLuaFuncs ( state );
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
	
	//----------------------------------------------------------------//
	void OnGfxEvent ( u32 event, void* userdata ) {
	
		// let Lua know the resource is ready for use
		this->InvokeListener ( ZLAbstractGfxResource::GFX_EVENT_CREATED );
		ZL_RESOURCE_TYPE::OnGfxEvent ( event, userdata );
	}
};

#endif
