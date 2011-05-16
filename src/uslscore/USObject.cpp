// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USCanary.h>
#include <uslscore/USObject.h>

//================================================================//
// USObject
//================================================================//

//----------------------------------------------------------------//
USCanary* USObject::AffirmCanary () {

	if ( !this->mCanary ) {
		this->mCanary = new USCanary ();
		this->mCanary->mObject = this;
	}
	return this->mCanary;
}

//----------------------------------------------------------------//
u32 USObject::GetRefCount () {

	if ( this->mCanary ) {
		return this->mCanary->mStrongRefs;
	}
	return 0;
}

//----------------------------------------------------------------//
bool USObject::IsInScope () {

	if ( this->mCanary ) {
		return ( this->mCanary->mStrongRefs > 0 );
	}
	return false;
}

//----------------------------------------------------------------//
void USObject::OnRelease ( u32 refCount ) {

	if ( refCount == 0 ) {
		delete this;
	}
}

//----------------------------------------------------------------//
void USObject::OnRetain ( u32 refCount ) {
	UNUSED ( refCount );
}

//----------------------------------------------------------------//
void USObject::Release () {

	if ( this->mCanary ) {
		this->mCanary->Release ( true );
	}
}

//----------------------------------------------------------------//
void USObject::Retain () {

	this->AffirmCanary ();
	this->mCanary->Retain ( true );
}

//----------------------------------------------------------------//
USObject::USObject () :
	mCanary ( 0 ) {
}

//----------------------------------------------------------------//
USObject::~USObject () {

	if ( this->mCanary ) {
		this->mCanary->mObject = 0;
	}
}
