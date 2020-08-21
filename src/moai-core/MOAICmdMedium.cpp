// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractCmdAPI.h>
#include <moai-core/MOAIAbstractCmdStream.h>
#include <moai-core/MOAICmdMedium.h>

//================================================================//
// MOAICmdMedium
//================================================================//

//----------------------------------------------------------------//
MOAICmdMedium::MOAICmdMedium ( MOAIAbstractCmdStream& stream ) :
	MOAICmdMediumPolymorphic ( stream ) {
	this->mAdapter = this;
}

//----------------------------------------------------------------//
MOAICmdMedium::~MOAICmdMedium () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void* MOAICmdMedium::MOAIAbstractCmdMedium_GetMediumWithAPI ( ZLTypeID apiTypeID ) {

	return ( this->mAPITypeID == apiTypeID ) ? ( void* )this->mBuffer.GetBuffer () : NULL;
}

