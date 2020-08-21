// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAICmdMediumPolymorphic.h>

//================================================================//
// MOAICmdMediumPolymorphic
//================================================================//
	
//----------------------------------------------------------------//
MOAICmdMediumPolymorphic::MOAICmdMediumPolymorphic () {
}

//----------------------------------------------------------------//
MOAICmdMediumPolymorphic::MOAICmdMediumPolymorphic ( MOAIAbstractCmdStream& stream ) :
	MOAICmdMediumAdapter ( stream ) {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAICmdMediumPolymorphic* MOAICmdMediumPolymorphic::MOAICmdMediumAdapter_GetPolymorphic () {
	return this;
}
