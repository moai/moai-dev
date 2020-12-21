// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionHolder.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	clear
	@text	Remove all props from the layer's partition.
	
	@in		MOAIPartitionHolder self
	@out	nil
*/
int MOAIPartitionHolder::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHolder, "U" )

	if ( self->mPartition ) {
		self->mPartition->Clear ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getPartition
	@text	Returns the partition currently attached to this layer.
	
	@in		MOAIPartitionHolder self
	@out	MOAIPartition partition
*/
int	MOAIPartitionHolder::_getPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHolder, "U" )

	self->GetPartition ()->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setPartition
	@text	Sets a partition for the holder to use. The holder will automatically
			create a partition when the first hull is added if no partition
			has been set.
	
	@in		MOAIPartitionHolder self
	@in		MOAIPartition partition
	@out	nil
*/
int MOAIPartitionHolder::_setPartition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPartitionHolder, "U" )

	self->mPartition.Set ( *self, state.GetLuaObject < MOAIPartition >( 2, true ));

	return 0;
}

//================================================================//
// MOAIPartitionHolder
//================================================================//

//----------------------------------------------------------------//
MOAIPartition* MOAIPartitionHolder::GetPartition () {

	if ( !this->mPartition ) {
		this->mPartition.Set ( *this, new MOAIPartition ());
	}
	assert ( this->mPartition );
	return this->mPartition;
}

//----------------------------------------------------------------//
MOAIPartitionHolder::MOAIPartitionHolder () {
	
	RTTI_BEGIN ( MOAIPartitionHolder )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIPartitionHolder >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIPartitionHolder::~MOAIPartitionHolder () {

	this->mPartition.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionHolder::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIPartitionHolder::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "clear",						_clear },
		{ "getPartition",				_getPartition },
		{ "setPartition",				_setPartition },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
