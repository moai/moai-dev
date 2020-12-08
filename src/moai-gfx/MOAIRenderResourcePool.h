// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERRESOURCEPOOL_H
#define	MOAIRENDERRESOURCEPOOL_H

class MOAIFrameBufferGL;
class MOAIFrameBufferAttachmentGL;

//================================================================//
// MOAIRenderResource
//================================================================//
// TODO: doxygen
class MOAIRenderResource :
	public virtual MOAILuaObject {
private:

	friend class MOAIRenderResourcePool;

	u32					mPoolType;

	//----------------------------------------------------------------//
	void				ZLRefCountedObjectBase_OnRelease			( u32 refCount );
	virtual void		MOAIRenderResource_OnRemit					();

public:

	static const u32 NOT_IN_POOL = ( u32 )-1;

	//----------------------------------------------------------------//
	bool				IsInPool									();
						MOAIRenderResource							();
						~MOAIRenderResource							();
	void				SetPoolType									( u32 poolType = NOT_IN_POOL );
};

//================================================================//
// MOAIRenderResourceFactory
//================================================================//
class MOAIRenderResourceFactory {
public:

	MOAILuaStrongRef		mFactory;
	MOAILuaStrongRef		mOnProvision;
	MOAILuaStrongRef		mOnRemit;
	
	//----------------------------------------------------------------//
	MOAIRenderResource* Create () {
		
		assert ( this->mFactory );
		
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mFactory );
		state.DebugCall ( 0, 1 );
		MOAIRenderResource* resource = state.GetLuaObject < MOAIRenderResource >( -1, false );

		return resource;
	}
	
	//----------------------------------------------------------------//
	void OnProvision ( MOAIRenderResource* resource ) {
	
		if ( !( this->mOnProvision && resource )) return;
		
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mOnProvision );
		state.Push ( resource );
		state.DebugCall ( 1, 0 );
		
	}
	
	//----------------------------------------------------------------//
	void OnRemit ( MOAIRenderResource* resource ) {
	
		if ( !( this->mOnRemit && resource )) return;
		
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mOnRemit );
		state.Push ( resource );
		state.DebugCall ( 1, 0 );
	}
};

//================================================================//
// MOAIRenderResourcePool
//================================================================//
// TODO: doxygen
class MOAIRenderResourcePool :
	public ZLContextClass < MOAIRenderResourcePool >,
	public virtual MOAILuaObject {
private:
	
	friend class MOAIRenderResource;
	
	STLMap < u32, MOAIRenderResourceFactory >	mFactory;
	STLSet < MOAIRenderResource* >				mResources;
	u32											mMaxID;
	
	STLMap < u32, STLSet < MOAIRenderResource* > >	mAvailableResourcesByType;
	
	//----------------------------------------------------------------//
	static int 				_addFactory						( lua_State* L );
	static int				_provision						( lua_State* L );
	
	//----------------------------------------------------------------//
	MOAIRenderResource* 	Provision						( u32 poolType );
	void					Remit							( MOAIRenderResource* resource );
	
	//----------------------------------------------------------------//
	void					_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					ZLContextClass_Finalize			();
	
public:
	
	DECL_LUA_SINGLETON ( MOAIRenderResourcePool )
	
	//----------------------------------------------------------------//
							MOAIRenderResourcePool			();
							~MOAIRenderResourcePool			();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static void ReleaseIfPooled ( ZLStrongPtr < TYPE >& ptr ) {
		MOAIRenderResource* resource = ptr;
		if ( resource && resource->IsInPool ()) {
			ptr = NULL;
		}
	}
};

#endif
