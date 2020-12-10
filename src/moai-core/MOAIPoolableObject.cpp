// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIPool.h>
#include <moai-core/MOAIPoolableObject.h>

//================================================================//
// MOAIPoolableObject
//================================================================//

//----------------------------------------------------------------//
bool MOAIPoolableObject::IsInPool () {

	return ( this->mPoolType != NOT_IN_POOL );
}

//----------------------------------------------------------------//
MOAIPoolableObject::MOAIPoolableObject () :
	mPoolType ( NOT_IN_POOL ) {
}

//----------------------------------------------------------------//
MOAIPoolableObject::~MOAIPoolableObject () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPoolableObject::ZLRefCountedObjectBase_OnRelease ( u32 refCount ) {

	if ( this->IsInPool () && ( refCount < 2 )) {
		if ( refCount == 0 ) {
			this->Retain ();
		}
		MOAIPool::Get ().Remit ( this );
	}
	MOAILuaObject::ZLRefCountedObjectBase_OnRelease ( refCount );
}

//----------------------------------------------------------------//
void MOAIPoolableObject::ZLRefCountedObjectBase_OnRetain ( u32 refCount ) {

	MOAILuaObject::ZLRefCountedObjectBase_OnRetain ( refCount );
}

//----------------------------------------------------------------//
void MOAIPoolableObject::MOAIRenderResource_OnRemit () {
}
