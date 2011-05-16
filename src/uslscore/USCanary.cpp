// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USCanary.h>
#include <uslscore/USFactory.h>
#include <uslscore/USObject.h>

//================================================================//
// USCanary
//================================================================//

//----------------------------------------------------------------//
void USCanary::ForceDelete () {

	if ( this->mObject ) {
		delete this->mObject;
	}
	this->mObject = 0;
}

//----------------------------------------------------------------//
USObject* USCanary::GetObject () {

	return this->mObject;
}

//----------------------------------------------------------------//
bool USCanary::IsValid () {

	if ( this->mObject  ) {
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void USCanary::Release ( bool strong ) {

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
void USCanary::Retain ( bool strong ) {

	++this->mRefCount;
	if ( strong ) {
		++this->mStrongRefs;
		
		if ( this->mObject ) {
			this->mObject->OnRetain ( this->mStrongRefs );
		}
	}
}

//----------------------------------------------------------------//
USCanary::USCanary () :
	mRefCount ( 0 ),
	mStrongRefs ( 0 ),
	mObject ( 0 ) {
}

//----------------------------------------------------------------//
USCanary::~USCanary () {

	if ( this->mObject ) {
		this->mObject->mCanary = 0;
	}
}
