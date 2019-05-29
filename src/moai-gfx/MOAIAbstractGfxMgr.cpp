// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIAbstractGfxMgr.h>

//================================================================//
// MOAIAbstractGfxMgr
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractShader* MOAIAbstractGfxMgr::AffirmShader ( MOAILuaState& state, int idx ) const {

	return this->MOAIAbstractGfxMgr_AffirmShader ( state, idx );
}

//----------------------------------------------------------------//
MOAIAbstractTexture* MOAIAbstractGfxMgr::AffirmTexture ( MOAILuaState& state, int idx ) const {

	return this->MOAIAbstractGfxMgr_AffirmTexture ( state, idx );
}

//----------------------------------------------------------------//
MOAIAbstractGfxMgr::MOAIAbstractGfxMgr () {
	
	RTTI_BEGIN
		RTTI_SINGLE ( MOAIGlobalEventSource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractGfxMgr::~MOAIAbstractGfxMgr () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxMgr& MOAIAbstractGfxMgr::MOAIAbstractGfxMgrComponents_GetAbstractGfxMgr () {
	return *this;
}

//----------------------------------------------------------------//
MOAIAbstractGfxStateGPUCache& MOAIAbstractGfxMgr::MOAIAbstractGfxMgrComponents_GetAbstractGfxStateGPUCache () {
	return *this;
}

//----------------------------------------------------------------//
MOAIAbstractVertexCache& MOAIAbstractGfxMgr::MOAIAbstractGfxMgrComponents_GetAbstractVertexCache () {
	return *this;
}

//----------------------------------------------------------------//
MOAIGfxStateCPUCache& MOAIAbstractGfxMgr::MOAIAbstractGfxMgrComponents_GetGfxStateCPUCache () {
	return *this;
}

