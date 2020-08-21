// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTCMDMEDIUM_H
#define	MOAIABSTRACTCMDMEDIUM_H

#include <moai-core/MOAIAbstractCmdHandler.h>
#include <moai-core/MOAILua.h>

class MOAIAbstractCmdAPI;
class MOAIAbstractCmdHandler;
class MOAIAbstractCmdMedium;
class MOAIAbstractCmdStream;
class MOAICmdMedium;
class MOAICmdMediumAdapter;
class MOAICmdMediumPolymorphic;
template < typename TYPE > class MOAICmdMediumWithAPI;

#define MOAI_LUA_CMD_SETUP(type)																	\
	MOAILuaState state ( L );																		\
	MOAICmdMediumWithAPI < type >* self = MOAIAbstractCmdMedium::LuaSetup < type >( state );		\
	if ( !self ) return 0;

//================================================================//
// MOAIAbstractCmdMedium
//================================================================//
class MOAIAbstractCmdMedium {
protected:

	friend class MOAIAbstractCmdHandler;

	MOAICmdMediumAdapter*	mAdapter;

	//----------------------------------------------------------------//
	static int			_getInterface					( lua_State* L );

	//----------------------------------------------------------------//
	void				PushLuaCmdAPI					( MOAILuaState& state );
	void				RetainObject					( MOAILuaObject* object );
	void				SubmitCommand					( u32 cmd, const void* param = NULL, ZLSize paramSize = 0 );

	//----------------------------------------------------------------//
	virtual void		MOAIAbstractCmdMedium_ExtendLuaAPI			( MOAILuaState& state );
	virtual void*		MOAIAbstractCmdMedium_GetMediumWithAPI		( ZLTypeID apiTypeID ) = 0;
	virtual void		MOAIAbstractCmdMedium_RetainObject			( MOAILuaObject* object );
	virtual void		MOAIAbstractCmdMedium_SubmitCommand			( u32 cmd, const void* param, ZLSize paramSize );

public:

	//----------------------------------------------------------------//
						MOAIAbstractCmdMedium			();
	virtual				~MOAIAbstractCmdMedium			();
	void				PushCmdInterface				( MOAILuaState& state );
	int					PushCmdInterfaceWithHandler		( MOAILuaState& state, int idx );
	void				PushCmdInterfaceWithHandler		( MOAILuaState& state, MOAIAbstractCmdHandler& handler );
	void				SetHandler						( MOAIAbstractCmdHandler& handler );
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	MOAICmdMediumWithAPI < API_TYPE >& _ ( MOAIAbstractCmdHandler& handler ) {

		this->SetHandler ( handler );
		return *this->API < API_TYPE >();
	}
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	MOAICmdMediumWithAPI < API_TYPE >* API () {
		
		return ( MOAICmdMediumWithAPI < API_TYPE >* )this->MOAIAbstractCmdMedium_GetMediumWithAPI ( ZLType::GetID < API_TYPE >());
	}
	
	//----------------------------------------------------------------//
	template < typename API_TYPE >
	static MOAICmdMediumWithAPI < API_TYPE >* LuaSetup ( MOAILuaState& state ) {
	
		if ( state.IsType ( 1, LUA_TUSERDATA )) {
			MOAIAbstractCmdMedium* self = ( MOAIAbstractCmdMedium* )state.GetPtrUserData ( 1 );
			return self->API < API_TYPE >();
		}
		return NULL;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void SubmitCommand ( u32 cmd, const TYPE& param ) {
		this->SubmitCommand ( cmd, &param, sizeof ( TYPE ));
	}
};

#endif
