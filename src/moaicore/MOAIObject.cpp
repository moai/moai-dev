// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICanary.h>
#include <moaicore/MOAIObject.h>

//================================================================//
// MOAIObject
//================================================================//

//----------------------------------------------------------------//
MOAICanary* MOAIObject::AffirmCanary () {

	if ( !this->mCanary ) {
		this->mCanary = new MOAICanary ();
		this->mCanary->mObject = this;
	}
	return this->mCanary;
}

//----------------------------------------------------------------//
u32 MOAIObject::GetRefCount () {

	if ( this->mCanary ) {
		return this->mCanary->mStrongRefs;
	}
	return 0;
}

//----------------------------------------------------------------//
bool MOAIObject::IsInScope () {

	if ( this->mCanary ) {
		return ( this->mCanary->mStrongRefs > 0 );
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIObject::OnRelease ( u32 refCount ) {

	if ( refCount == 0 ) {
		delete this;
	}
}

//----------------------------------------------------------------//
void MOAIObject::OnRetain ( u32 refCount ) {
	UNUSED ( refCount );
}

//----------------------------------------------------------------//
void MOAIObject::Release () {

	if ( this->mCanary ) {
		this->mCanary->Release ( true );
	}
}

//----------------------------------------------------------------//
void MOAIObject::Retain () {

	this->AffirmCanary ();
	this->mCanary->Retain ( true );
}

//----------------------------------------------------------------//
MOAIObject::MOAIObject () :
	mCanary ( 0 ) {
}

//----------------------------------------------------------------//
MOAIObject::~MOAIObject () {

	if ( this->mCanary ) {
		this->mCanary->mObject = 0;
	}
}
