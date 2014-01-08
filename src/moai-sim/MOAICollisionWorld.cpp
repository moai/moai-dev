// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICollisionWorld.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIProp.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clear
	@text	Remove all props from the world's partition.
	
	@in		MOAICollisionWorld self
	@out	nil
*/
int MOAICollisionWorld::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionWorld, "U" )

	if ( self->mPartition ) {
		self->mPartition->Clear ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getPartition
	@text	Returns the partition (if any) currently attached to this world.
	
	@in		MOAICollisionWorld self
	@out	MOAIPartition partition
*/
int	MOAICollisionWorld::_getPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionWorld, "U" )

	self->AffirmPartition ();
	self->mPartition->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	insertProp
	@text	Adds a prop to the world's partition.
	
	@in		MOAICollisionWorld self
	@in		MOAIProp prop
	@out	nil
*/
int	MOAICollisionWorld::_insertProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionWorld, "UU" )

	MOAIProp* prop = state.GetLuaObject < MOAIProp >( 2, true );
	if ( !prop ) return 0;

	self->AffirmPartition ();
	self->mPartition->InsertProp ( *prop );
	prop->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	removeProp
	@text	Removes a prop from the world's partition.
	
	@in		MOAICollisionWorld self
	@in		MOAIProp prop
	@out	nil
*/
int	MOAICollisionWorld::_removeProp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionWorld, "UU" )

	MOAIProp* prop = state.GetLuaObject < MOAIProp >( 2, true );
	if ( !prop ) return 0;

	if ( self->mPartition ) {
		self->mPartition->RemoveProp ( *prop );
		prop->ScheduleUpdate ();
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPartition
	@text	Sets a partition for the world to use. The world will automatically
			create a partition when the first prop is added if no partition
			has been set.
	
	@in		MOAICollisionWorld self
	@in		MOAIPartition partition
	@out	nil
*/
int MOAICollisionWorld::_setPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICollisionWorld, "UU" )

	self->mPartition.Set ( *self, state.GetLuaObject < MOAIPartition >( 2, true ));

	return 0;
}

//================================================================//
// MOAINode
//================================================================//

//----------------------------------------------------------------//
void MOAICollisionWorld::AffirmPartition () {

	if ( !this->mPartition ) {
		this->mPartition.Set ( *this, new MOAIPartition ());
	}
}

//----------------------------------------------------------------//
MOAICollisionWorld::MOAICollisionWorld () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
}

//----------------------------------------------------------------//
MOAICollisionWorld::~MOAICollisionWorld () {

	this->mPartition.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "clear",					_clear },
		{ "getPartition",			_getPartition },
		{ "insertProp",				_insertProp },
		{ "removeProp",				_removeProp },
		{ "setPartition",			_setPartition },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIAction::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAICollisionWorld::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIAction::SerializeOut ( state, serializer );
}