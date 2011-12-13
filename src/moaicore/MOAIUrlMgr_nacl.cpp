// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIHttpTask.h>
#include <moaicore/MOAIHttpTaskInfo_nacl.h>
#include <moaicore/MOAIUrlMgr.h>

SUPPRESS_EMPTY_FILE_WARNING
#ifdef MOAI_OS_NACL

//================================================================//
// MOAIUrlMgr
//================================================================//

static bool sMore = false;
static STLList < MOAIHttpTask* > sTasks;

//----------------------------------------------------------------//
void MOAIUrlMgr::AddHandle ( MOAIHttpTask& task ) {
	
	sTasks.push_back ( &task );
}

//----------------------------------------------------------------//
bool MOAIUrlMgr::More () {

	return false;
}

//----------------------------------------------------------------//
void MOAIUrlMgr::Process () {

	for( STLList < MOAIHttpTask* >::iterator iter = sTasks.begin(); iter != sTasks.end(); ++iter )
	{
		if(( *iter )->GetInfo ()->mReady ) {

			( *iter )->Finish ();

			STLList < MOAIHttpTask* >::iterator old_iter = iter;
			old_iter--;
			sTasks.erase ( iter );
			iter = old_iter;
		}
	}
}

//----------------------------------------------------------------//
MOAIUrlMgr::MOAIUrlMgr () {
}

//----------------------------------------------------------------//
MOAIUrlMgr::~MOAIUrlMgr () {
}

#endif
