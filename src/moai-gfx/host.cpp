// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/host.h>
#include <moai-gfx/headers.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUGfxAppFinalize () {
}

//----------------------------------------------------------------//
void AKUGfxAppInitialize () {
}

//----------------------------------------------------------------//
void AKUGfxContextInitialize ( AKUContextID contextID ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	context->Affirm < MOAIDraw >();
	context->Affirm < MOAIVertexFormatMgr >();
	
	REGISTER_LUA_CLASS ( context, MOAICamera )
	REGISTER_LUA_CLASS ( context, MOAIDraw )
	REGISTER_LUA_CLASS ( context, MOAIMatrix )
	REGISTER_LUA_CLASS ( context, MOAIRenderNode )
	REGISTER_LUA_CLASS ( context, MOAITransform )
	REGISTER_LUA_CLASS ( context, MOAIVertexFormatMgr )
	REGISTER_LUA_CLASS ( context, MOAIViewport )
}
