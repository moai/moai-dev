// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILuaCanary.h>
#include <moai-core/MOAILuaObject.h>

//================================================================//
// MOAILuaCanary
//================================================================//

//----------------------------------------------------------------//
bool MOAILuaCanary::IsValid () {
	return this->mIsValid;
}

//----------------------------------------------------------------//
MOAILuaCanary::MOAILuaCanary () :
	mRefCount ( 0 ),
	mIsValid ( true ) {
}

//----------------------------------------------------------------//
MOAILuaCanary::~MOAILuaCanary () {
}

//----------------------------------------------------------------//
void MOAILuaCanary::Release () {
	if ( this->mRefCount ) {
		this->mRefCount--;
		if ( this->mRefCount == 0 ) {
			delete this;
		}
	} 
}

//----------------------------------------------------------------//
void MOAILuaCanary::Retain () {
	this->mRefCount++;
}
