// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIScope.h>
#include <moai-core/MOAIPoolableObject.h>
#include <moai-core/MOAIPoolableObjectFactory.h>

//================================================================//
// MOAIPoolableObjectFactory
//================================================================//
	
//----------------------------------------------------------------//
MOAIPoolableObject* MOAIPoolableObjectFactory::Create () {
	
	assert ( this->mFactory );
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	state.Push ( this->mFactory );
	state.DebugCall ( 0, 1 );
	MOAIPoolableObject* resource = state.GetLuaObject < MOAIPoolableObject >( -1, false );

	return resource;
}

//----------------------------------------------------------------//
void MOAIPoolableObjectFactory::OnProvision ( MOAIPoolableObject& resource, MOAIScope& scope ) {

	if ( !( this->mOnProvision )) return;
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	state.Push ( this->mOnProvision );
	state.Push ( resource );
	state.Push ( scope );
	state.DebugCall ( 2, 0 );
}

//----------------------------------------------------------------//
void MOAIPoolableObjectFactory::OnRemit ( MOAIPoolableObject& resource ) {

	if ( !( this->mOnRemit )) return;
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	state.Push ( this->mOnRemit );
	state.Push ( &resource );
	state.DebugCall ( 1, 0 );
}
