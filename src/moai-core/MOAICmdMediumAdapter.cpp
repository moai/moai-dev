// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAICmdMediumAdapter.h>
#include <moai-core/MOAIAbstractCmdStream.h>

//================================================================//
// MOAICmdMediumAdapter
//================================================================//

//----------------------------------------------------------------//
MOAICmdMediumPolymorphic& MOAICmdMediumAdapter::GetPolymorphic () {

	return *this->MOAICmdMediumAdapter_GetPolymorphic ();
}

//----------------------------------------------------------------//
MOAICmdMediumAdapter::MOAICmdMediumAdapter () :
	mStream ( NULL ),
	mHandler ( NULL ),
	mAPI ( NULL ) {
}

//----------------------------------------------------------------//
MOAICmdMediumAdapter::MOAICmdMediumAdapter ( MOAIAbstractCmdStream& stream ) :
	mStream ( &stream ),
	mHandler ( NULL ),
	mAPI ( NULL ) {
}

//----------------------------------------------------------------//
MOAICmdMediumAdapter::~MOAICmdMediumAdapter () {
}

//----------------------------------------------------------------//
void MOAICmdMediumAdapter::RetainObject ( MOAILuaObject* object ) {
	this->mStream->RetainObject ( object );
}

//----------------------------------------------------------------//
void MOAICmdMediumAdapter::SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {
	this->mStream->SubmitCommand ( *this->mHandler, cmd, param, paramSize );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAICmdMediumPolymorphic* MOAICmdMediumAdapter::MOAICmdMediumAdapter_GetPolymorphic () {
	assert ( false );
	return NULL;
}
