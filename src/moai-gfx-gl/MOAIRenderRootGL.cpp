// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIRenderRootGL.h>

//================================================================//
// MOAIRenderRootGL
//================================================================//

//----------------------------------------------------------------//
MOAIRenderRootGL::MOAIRenderRootGL () {

	RTTI_BEGIN ( MOAIAbstractLayer )
		RTTI_EXTEND ( MOAIRenderNode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIRenderRootGL::~MOAIRenderRootGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderRootGL::MOAIAbstractRenderNode_Render ( u32 renderPhase ) {

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.BeginFrame ();
	
	gfxMgr.SetFrameBuffer ();
	gfxMgr.SetViewRect ();
	gfxMgr.SetScissorRect ();

	gfxMgr.SetClearColor ( 0x00000000 );
	gfxMgr.SetClearFlags ( MOAIClearFlagsEnum::COLOR_BUFFER_BIT | MOAIClearFlagsEnum::DEPTH_BUFFER_BIT );
	gfxMgr.ClearSurface ();
	
	this->MOAIRenderNode::MOAIAbstractRenderNode_Render ( renderPhase );
	
	gfxMgr.EndFrame ();
}
