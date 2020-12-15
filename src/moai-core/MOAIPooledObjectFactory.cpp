// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIScope.h>
#include <moai-core/MOAIPooledObjectFactory.h>

//================================================================//
// MOAIPooledObjectFactory
//================================================================//
	
//----------------------------------------------------------------//
MOAILuaObject* MOAIPooledObjectFactory::Create () {
	
	assert ( this->mFactory );
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	state.Push ( this->mFactory );
	state.DebugCall ( 0, 1 );
	MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( -1, false );

	return object;
}

//----------------------------------------------------------------//
void MOAIPooledObjectFactory::OnProvision ( MOAILuaObject& object, MOAIScope& scope ) {

	if ( !( this->mOnProvision )) return;
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	state.Push ( this->mOnProvision );
	state.Push ( object );
	state.Push ( scope );
	state.DebugCall ( 2, 0 );
}

//----------------------------------------------------------------//
void MOAIPooledObjectFactory::OnRemit ( MOAILuaObject& object ) {

	if ( !( this->mOnRemit )) return;
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	state.Push ( this->mOnRemit );
	state.Push ( &object );
	state.DebugCall ( 1, 0 );
}
