// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIHttpTaskNaCl.h>
#include <moaicore/MOAIUrlMgrNaCl.h>

SUPPRESS_EMPTY_FILE_WARNING
#ifdef MOAI_OS_NACL

//================================================================//
// MOAIUrlMgrNaCl
//================================================================//

//----------------------------------------------------------------//
void MOAIUrlMgrNaCl::AddHandle ( MOAIHttpTaskNaCl& task ) {
	
	this->mTasks.push_back ( &task );
}

//----------------------------------------------------------------//
void MOAIUrlMgrNaCl::Process () {

	for( STLList < MOAIHttpTaskNaCl* >::iterator iter = this->mTasks.begin(); iter != this->mTasks.end(); ++iter ) {
	
		if(( *iter )->IsReady () ) {

			( *iter )->NaClFinish ();
			( *iter )->Release ();

			STLList < MOAIHttpTaskNaCl* >::iterator old_iter = iter;
			old_iter--;
			this->mTasks.erase ( iter );
			iter = old_iter;
		}
	}
}

//----------------------------------------------------------------//
MOAIUrlMgrNaCl::MOAIUrlMgrNaCl () {
}

//----------------------------------------------------------------//
MOAIUrlMgrNaCl::~MOAIUrlMgrNaCl () {
}

#endif
