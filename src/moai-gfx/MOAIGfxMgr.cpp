// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgr.h>

//================================================================//
// MOAIGfxMgr
//================================================================//

//----------------------------------------------------------------//
MOAIShader* MOAIGfxMgr::AffirmShader ( MOAILuaState& state, int idx ) const {

	return this->MOAIAbstractGfxMgr_AffirmShader ( state, idx );
}

//----------------------------------------------------------------//
MOAITexture* MOAIGfxMgr::AffirmTexture ( MOAILuaState& state, int idx ) const {

	return this->MOAIAbstractGfxMgr_AffirmTexture ( state, idx );
}

//----------------------------------------------------------------//
MOAIGfxMgr::MOAIGfxMgr () {
	
	RTTI_BEGIN
		RTTI_SINGLE ( MOAIGlobalEventSource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxMgr::~MOAIGfxMgr () {
}

//----------------------------------------------------------------//
void MOAIGfxMgr::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIGfxMgr_RenderTree::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGfxMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIGfxMgr_RenderTree::RegisterLuaFuncs ( state );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgr_CPUCache& MOAIGfxMgr::MOAIAbstractGfxMgrComponents_GetCPUCache () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgr& MOAIGfxMgr::MOAIAbstractGfxMgrComponents_GetGfxMgr () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgr_GPUCache& MOAIGfxMgr::MOAIAbstractGfxMgrComponents_GetGPUCache () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgr_RenderTree& MOAIGfxMgr::MOAIAbstractGfxMgrComponents_GetRenderTree () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxMgr_VertexCache& MOAIGfxMgr::MOAIAbstractGfxMgrComponents_GetVertexCache () {
	return *this;
}
