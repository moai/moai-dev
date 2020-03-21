// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractGfxComposerCallable.h>
#include <moai-gfx/MOAIAbstractUniformBuffer.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIGfxComposer.h>
#include <moai-gfx/MOAIUniformSchema.h>

//================================================================//
// MOAIUniformComposerGlobalRef
//================================================================//

////----------------------------------------------------------------//
//MOAIUniformComposerGlobalRef::MOAIUniformComposerGlobalRef () :
//	mGlobalID ( ZLIndexOp::INVALID ),
//	mUniformID ( ZLIndexOp::INVALID ),
//	mIndex ( 0 ) {
//}
//
////================================================================//
//// MOAIUniformComposerTextureRef
////================================================================//
//
////----------------------------------------------------------------//
//MOAIUniformComposerTextureRef::MOAIUniformComposerTextureRef () :
//	mName ( MOAIMaterialGlobals::MOAI_UNKNOWN_MATERIAL_GLOBAL ),
//	mUnit ( 0 ) {
//}

//================================================================//
// MOAIGfxComposer
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxComposer::Execute ( MOAIAbstractGfxComposerCallable& callable, MOAIGfxComposerCmdEnum::_ callCommand ) {

	bool didCall = false;
	ZLSize size = this->mCommands.GetTop ();
	for ( ZLIndex index = 0; index < size; ++index ) {
		this->mCommands [ index ]->Execute ( callable );
	}
	
	if ( !didCall ) {
		MOAIGfxComposerCmd::ExecuteCall ( callable, callCommand );
	}
}

//----------------------------------------------------------------//
MOAIGfxComposer::MOAIGfxComposer () {

	RTTI_BEGIN ( MOAIGfxComposer )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxComposer >)
		RTTI_EXTEND ( MOAIGfxComposerInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxComposer::~MOAIGfxComposer () {
}

//----------------------------------------------------------------//
//void MOAIGfxComposer::SelectTextures () {
//
//	// TODO: this needs to go away. texture (in GL) are always.
//	// No reason (right now) to remap material manager texture names to
//	// different texture units. Should be part of the material,
//	// if it exists at all. Removing this, shaders cleanly break
//	// with material manager.
//
//	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
//	MOAIMaterialMgr& materialMgr = MOAIMaterialMgr::Get ();
//
//	size_t nTextures = this->mTextures.Size ();
//	for ( ZLIndex i = 0; i < nTextures; ++i ) {
//
//		MOAIUniformComposerTextureRef& textureRef = this->mTextures [ i ];
//
//		MOAITexture* texture = textureRef.mTexture;
//
//		// load texture by name
//		if ( !texture ) {
//			texture = materialMgr.GetTexture ( textureRef.mName );
//		}
//		gfxMgr.SetTexture ( texture, textureRef.mUnit );
//	}
//}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIGfxComposer& MOAIGfxComposer::MOAIGfxComposerInterface_AffirmComposer () {

	return *this;
}

//----------------------------------------------------------------//
MOAIGfxComposer* MOAIGfxComposer::MOAIGfxComposerInterface_GetComposer () {

	return this;
}
