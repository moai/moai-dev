// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIBlocker.h>

//================================================================//
// MOAIBlocker
//================================================================//

//----------------------------------------------------------------//
void MOAIBlocker::AddBlocked ( MOAIBlocker* blocked ) {
	
	blocked->mBlocker = this;
	blocked->mNextBlocked = this->mBlockedList;
	this->mBlockedList = blocked;
}

//----------------------------------------------------------------//
bool MOAIBlocker::IsBlocked () {

	if ( this->mBlocker ) {
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
MOAIBlocker::MOAIBlocker () :
	mBlocker ( 0 ),
	mBlockedList ( 0 ),
	mNextBlocked ( 0 ) {
}

//----------------------------------------------------------------//
MOAIBlocker::~MOAIBlocker () {

	this->UnblockSelf ();
	this->UnblockAll ();
}

//----------------------------------------------------------------//
void MOAIBlocker::OnUnblock () {
}

//----------------------------------------------------------------//
void MOAIBlocker::RemoveBlocked ( MOAIBlocker* blocked ) {

	MOAIBlocker* list = 0;

	MOAIBlocker* cursor = this->mBlockedList;
	while ( cursor ) {
	
		MOAIBlocker* temp = cursor;
		cursor = cursor->mNextBlocked;
	
		if ( temp == blocked ) {
			temp->mBlocker = 0;
			temp->mNextBlocked = 0;
			temp->OnUnblock ();
		}
		else {
			temp->mNextBlocked = list;
			list = temp;
		}
	}
	this->mBlockedList = list;
}

//----------------------------------------------------------------//
void MOAIBlocker::SetBlocker ( MOAIBlocker* blocker ) {

	if ( this->mBlocker ) {
		if ( this->mBlocker == blocker ) return;
		this->mBlocker->RemoveBlocked ( this );
	}
	
	if ( blocker ) {
		blocker->AddBlocked ( this );
	}
}

//----------------------------------------------------------------//
void MOAIBlocker::UnblockAll () {

	MOAIBlocker* cursor = this->mBlockedList;
	for ( ; cursor; cursor = cursor->mNextBlocked ) {
		cursor->mBlocker = 0;
	}
	this->mBlockedList = 0;
}

//----------------------------------------------------------------//
void MOAIBlocker::UnblockSelf () {

	if ( this->mBlocker ) {
		this->mBlocker->RemoveBlocked ( this );
	}
}
