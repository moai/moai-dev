// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICanary.h>
#include <moaicore/MOAIObject.h>

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
	
		if ( this->mStrongRefs ) {
			--this->mStrongRefs;
		}
		
		if ( this->mObject ) {
			this->mObject->OnRelease ( this->mStrongRefs );
		}
	}
	
	if ( this->mRefCount ) {
		--this->mRefCount;
		
		if ( this->mRefCount == 0 ) {
			delete this;
		}
	}
}

//----------------------------------------------------------------//
void MOAICanary::Retain ( bool strong ) {

	++this->mRefCount;
	if ( strong ) {
		++this->mStrongRefs;
		
		if ( this->mObject ) {
			this->mObject->OnRetain ( this->mStrongRefs );
		}
	}
}

//----------------------------------------------------------------//
MOAICanary::MOAICanary () :
	mRefCount ( 0 ),
	mStrongRefs ( 0 ),
	mObject ( 0 ) {
}

//----------------------------------------------------------------//
MOAICanary::~MOAICanary () {

	if ( this->mObject ) {
		this->mObject->mCanary = 0;
	}
}
