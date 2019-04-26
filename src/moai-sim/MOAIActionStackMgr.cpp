// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIActionStackMgr.h>

//================================================================//
// MOAIActionStackMgr
//================================================================//
	
//----------------------------------------------------------------//
MOAIAction* MOAIActionStackMgr::GetCurrent () {

	ZLSize top = this->mStack.GetTop ();
	return top > 0 ? this->mStack [ ZLIndexCast ( top - 1 )] : 0;
}

//----------------------------------------------------------------//
MOAIAction* MOAIActionStackMgr::GetDefaultParent () {

	MOAIAction* defaultParent = 0;

	MOAIAction* cursor = this->GetCurrent ();
	for ( ; cursor; cursor = cursor->mParent ) {
		defaultParent = cursor->GetDefaultParent ();
		if ( defaultParent ) {
			return defaultParent;
		}
	}
	return MOAISim::Get ().GetActionMgr ().GetDefaultParent ();
}

//----------------------------------------------------------------//
MOAIActionStackMgr::MOAIActionStackMgr () {
}

//----------------------------------------------------------------//
MOAIActionStackMgr::~MOAIActionStackMgr () {
}

//----------------------------------------------------------------//
void MOAIActionStackMgr::Pop () {

	this->mStack.Pop ();
}

//----------------------------------------------------------------//
void MOAIActionStackMgr::Push ( MOAIAction& action ) {

	this->mStack.Push ( &action );
}
