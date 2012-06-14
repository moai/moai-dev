// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USDataIOTask.h>

//================================================================//
// USDataIOTask
//================================================================//

//----------------------------------------------------------------//
void USDataIOTask::Execute () {

	if ( this->mState == LOADING ) { 
		this->mData->Load ( this->mFilename );
	}
	else if ( this->mState == SAVING ) {
		this->mData->Save ( this->mFilename );
	}
	
	this->mState = IDLE;
}

//----------------------------------------------------------------//
void USDataIOTask::LoadData ( cc8* filename, USData& target ) {

	if ( this->mState == IDLE ) {

		this->SetFilename ( filename );
		this->SetData ( &target );
		
		this->mState = LOADING;
		this->Start ();
	}
}

//----------------------------------------------------------------//
void USDataIOTask::SaveData ( cc8* filename, USData& target ) {

	if ( this->mState == IDLE ) {

		this->SetFilename ( filename );
		this->SetData ( &target );
		
		this->mState = SAVING;
		this->Start ();
	}
}

//----------------------------------------------------------------//
USDataIOTask::USDataIOTask () :
	mState ( IDLE ) {
}

//----------------------------------------------------------------//
USDataIOTask::~USDataIOTask () {
}
