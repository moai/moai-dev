// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAICanary.h>
#include <moai-core/MOAIObject.h>

//================================================================//
// MOAICanary
//================================================================//

//----------------------------------------------------------------//
void MOAICanary::ForceDelete () {

	if ( this->mObject ) {
		delete this->mObject;
	}
	this->mObject = 0;
}

//----------------------------------------------------------------//
MOAIObject* MOAICanary::GetObject () {

	return this->mObject;
}

//----------------------------------------------------------------//
bool MOAICanary::IsValid () {

	if ( this->mObject  ) {
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAICanary::Release ( bool strong ) {

	if ( strong ) {
	
		if ( this->mObjectRefCount ) {
			--this->mObjectRefCount;
		}
		
		if ( this->mObject ) {
			this->mObject->OnRelease ( this->mObjectRefCount );
		}
	}
	
	if ( this->mCanaryRefCount ) {
		--this->mCanaryRefCount;
		
		if ( this->mCanaryRefCount == 0 ) {
			delete this;
		}
	}
}

//----------------------------------------------------------------//
void MOAICanary::Retain ( bool strong ) {

	++this->mCanaryRefCount;
	if ( strong ) {
		++this->mObjectRefCount;
		
		if ( this->mObject ) {
			this->mObject->OnRetain ( this->mObjectRefCount );
		}
	}
}

//----------------------------------------------------------------//
MOAICanary::MOAICanary () :
	mObjectRefCount ( 0 ),
	mCanaryRefCount ( 0 ),
	mObject ( 0 ) {
}

//----------------------------------------------------------------//
MOAICanary::~MOAICanary () {

	if ( this->mObject ) {
		this->mObject->mCanary = 0;
	}
}
