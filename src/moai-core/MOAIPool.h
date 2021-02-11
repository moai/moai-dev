// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPOOL_H
#define	MOAIPOOL_H

#include <moai-core/MOAILua.h>
#include <moai-core/MOAIPooledObjectFactory.h>

class MOAIScope;

//================================================================//
// MOAIPool
//================================================================//
// TODO: doxygen
class MOAIPool :
	public ZLContextClass < MOAIPool >,
	public virtual MOAILuaObject {
private:
	
	friend class MOAILuaObject;
	
	static const u32 STARTING_ID = 0x70000000;
	
	STLMap < u32, ZLStrongPtr < MOAIAbstractPooledObjectFactory > > mFactory;
	STLSet < MOAILuaObject* >					mResources;
	u32											mMaxID;
	
	STLMap < u32, STLSet < MOAILuaObject* > >	mAvailableResourcesByType;
	
	//----------------------------------------------------------------//
	static int 				_addFactory						( lua_State* L );
	static int				_provision						( lua_State* L );
	static int				_remit							( lua_State* L );
	
	//----------------------------------------------------------------//
	MOAILuaObject* 			Provision						( u32 poolType, MOAIScope* scope );
	void					Remit							( MOAILuaObject* resource );
	
	//----------------------------------------------------------------//
	void					_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	void					ZLContextClass_Finalize			();
	
public:
	
	DECL_LUA_SINGLETON ( MOAIPool )
	
	//----------------------------------------------------------------//
							MOAIPool			();
							~MOAIPool			();
	void					PurgeAll			();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void AffirmFactory () {
	
		u32 typeID = ( u32 )ZLType::RawID < TYPE >();
		assert ( typeID < STARTING_ID );
	
		if ( !this->mFactory.contains ( typeID )) {
			this->mFactory [ typeID ] = new MOAIPooledObjectFactory < TYPE >();
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void Provision ( ZLStrongPtr < TYPE >& retain, MOAIScope* scope = NULL ) {
	
		u32 typeID = ( u32 )ZLType::RawID < TYPE >();
		this->AffirmFactory < TYPE >();
		retain = this->Provision ( typeID, scope )->AsType < TYPE >(); // TODO: cache this?
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	static void ReleaseIfPooled ( ZLStrongPtr < TYPE >& ptr ) {
		MOAILuaObject* resource = ptr;
		if ( resource && resource->IsInPool ()) {
			ptr = NULL;
		}
	}
};

#endif
