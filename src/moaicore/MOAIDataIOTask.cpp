// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataIOTask.h>

//================================================================//
// MOAIDataIOTask
//================================================================//

//----------------------------------------------------------------//
void MOAIDataIOTask::Execute () {

	if ( this->mState == LOADING ) { 
		this->mData->Load ( this->mFilename );
	}
	else if ( this->mState == SAVING ) {
		this->mData->Save ( this->mFilename );
	}
	
	this->mState = IDLE;
}

//----------------------------------------------------------------//
void MOAIDataIOTask::LoadData ( cc8* filename, MOAIDataBuffer& target ) {

	if ( this->mState == IDLE ) {

		this->SetFilename ( filename );
		this->SetData ( &target );
		
		this->mState = LOADING;
		this->Start ();
	}
}

//----------------------------------------------------------------//
MOAIDataIOTask::MOAIDataIOTask () :
	mState ( IDLE ) {
}

//----------------------------------------------------------------//
MOAIDataIOTask::~MOAIDataIOTask () {
}

//----------------------------------------------------------------//
void MOAIDataIOTask::SaveData ( cc8* filename, MOAIDataBuffer& target ) {

	if ( this->mState == IDLE ) {

		this->SetFilename ( filename );
		this->SetData ( &target );
		
		this->mState = SAVING;
		this->Start ();
	}
}

