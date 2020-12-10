// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPOOL_H
#define	MOAIPOOL_H

#include <moai-core/MOAILua.h>
#include <moai-core/MOAIPoolableObject.h>
#include <moai-core/MOAIPoolableObjectFactory.h>

class MOAIScope;

//================================================================//
// MOAIPool
//================================================================//
// TODO: doxygen
class MOAIPool :
	public ZLContextClass < MOAIPool >,
	public virtual MOAILuaObject {
private:
	
	friend class MOAIPoolableObject;
	
	STLMap < u32, MOAIPoolableObjectFactory >		mFactory;
	STLSet < MOAIPoolableObject* >					mResources;
	u32										mMaxID;
	
	STLMap < u32, STLSet < MOAIPoolableObject* > >	mAvailableResourcesByType;
	
	//----------------------------------------------------------------//
	static int 				_addFactory						( lua_State* L );
	static int				_provision						( lua_State* L );
	
	//----------------------------------------------------------------//
	MOAIPoolableObject* 			Provision						( u32 poolType, MOAIScope& scope );
	void					Remit							( MOAIPoolableObject* resource );
	
	//----------------------------------------------------------------//
	void					_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					ZLContextClass_Finalize			();
	
public:
	
	DECL_LUA_SINGLETON ( MOAIPool )
	
	//----------------------------------------------------------------//
							MOAIPool			();
							~MOAIPool			();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static void ReleaseIfPooled ( ZLStrongPtr < TYPE >& ptr ) {
		MOAIPoolableObject* resource = ptr;
		if ( resource && resource->IsInPool ()) {
			ptr = NULL;
		}
	}
};

#endif
