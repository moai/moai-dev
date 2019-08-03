// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAILight.h>
#include <moai-gfx/MOAIMaterialMgr.h>

//================================================================//
// MOAIMaterialStackScope
//================================================================//

//----------------------------------------------------------------//
MOAIMaterialStackScope::MOAIMaterialStackScope () :
	mMaterialStack ( MOAIMaterialMgr::Get ()) {
	
	this->mRestoreTop = this->mMaterialStack.mStack.GetTop ();
}

//----------------------------------------------------------------//
MOAIMaterialStackScope::~MOAIMaterialStackScope () {

	u32 top = this->mMaterialStack.mStack.GetTop ();
	for ( ; top > this->mRestoreTop; --top ) {
		this->mMaterialStack.Pop ();
	}
}

//================================================================//
// MOAIMaterialGlobal
//================================================================//
	
//----------------------------------------------------------------//
MOAIMaterialGlobal::MOAIMaterialGlobal () :
	mPtr ( 0 ),
	mStackDepth ( 0 ) {
}

//----------------------------------------------------------------//
MOAIMaterialGlobal::~MOAIMaterialGlobal () {
}

//================================================================//
// MOAIMaterialMgr
//================================================================//

//----------------------------------------------------------------//
MOAIMaterialMgr::MOAIMaterialMgr () {

	this->mNamedLights.Init ( MOAIMaterialGlobals::MAX_GLOBAL_LIGHTS );
	this->mNamedTextures.Init ( MOAIMaterialGlobals::MAX_GLOBAL_TEXTURES );
	
	this->mOverwrite = false;
}

//----------------------------------------------------------------//
MOAIMaterialMgr::~MOAIMaterialMgr () {
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::Pop () {

	size_t stackTop = this->mStack.GetTop ();

	if ( stackTop ) {
	
		MOAIMaterialStackFrame frame = this->mStack.Pop ();
		
		if ( stackTop > 1 ) {
			this->Clear ( ~frame.mFlags );
		}
		else {
			this->mStack.Reset ();
			this->Clear ();
		}
		
		MOAIMaterialStackClearCmd* cursor = frame.mClearList;
		
		while ( cursor ) {
		
			MOAIMaterialStackClearCmd* clearCmd = cursor;
			cursor = cursor->mNext;
			
			MOAIMaterialGlobal* global = cursor->mGlobal;
			
			global->mPtr = 0;
			global->mStackDepth = 0;
			
			this->mRestoreCmdPool.Free ( clearCmd );
		}
	}
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::Push ( MOAIAbstractMaterial* material ) {
	
	MOAIMaterialStackFrame& frame = this->mStack.Push ();
	
	frame.mFlags = this->mFlags;
	frame.mClearList = 0;
	
	if ( material ) {
		this->Compose ( *material );
	}
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::SetGlobal ( MOAIMaterialGlobal& global, void* ptr ) {

	u32 stackDepth = ( u32 )this->mStack.GetTop ();
	assert ( stackDepth );

	if ( global.mStackDepth < stackDepth ) {
	
		if ( global.mPtr ) return; // don't overwrite
		
		MOAIMaterialStackClearCmd* clearCmd = this->mRestoreCmdPool.Alloc ();
		assert ( clearCmd );
		
		clearCmd->mGlobal = &global;
		
		MOAIMaterialStackFrame& frame = this->mStack.Top ();
		clearCmd->mNext = frame.mClearList;
		frame.mClearList = clearCmd->mNext;
	}
	
	global.mPtr = ptr;
	global.mStackDepth = stackDepth;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterialMgr::MOAIAbstractMaterial_ApplyGlobals ( MOAIAbstractMaterialInterface& dest ) {
	UNUSED ( dest );
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterialMgr::MOAIAbstractMaterial_ClearGlobals () {
}

//----------------------------------------------------------------//
MOAILight* MOAIMaterialMgr::MOAIAbstractMaterial_GetLight ( u32 name ) {

	assert ( name < MOAIMaterialGlobals::MAX_GLOBAL_TEXTURES );
	return this->mNamedLights [ ZLIndexCast ( name )].mLight;
}

//----------------------------------------------------------------//
MOAITexture* MOAIMaterialMgr::MOAIAbstractMaterial_GetTexture ( u32 name ) {

	assert ( name < MOAIMaterialGlobals::MAX_GLOBAL_TEXTURES );
	return this->mNamedTextures [ ZLIndexCast ( name )].mTexture;
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::MOAIAbstractMaterial_SetLight ( u32 name, MOAILight* light ) {

	assert ( name < MOAIMaterialGlobals::MAX_GLOBAL_LIGHTS );
	this->SetGlobal ( this->mNamedLights [ ZLIndexCast ( name )], light );
}

//----------------------------------------------------------------//
void MOAIMaterialMgr::MOAIAbstractMaterial_SetTexture ( u32 name, MOAITexture* texture ) {

	assert ( name < MOAIMaterialGlobals::MAX_GLOBAL_TEXTURES );
	this->SetGlobal ( this->mNamedTextures [ ZLIndexCast ( name )], texture );
}

//----------------------------------------------------------------//
MOAIAbstractMaterial& MOAIMaterialMgr::MOAIAbstractMaterialInterface_AffirmMaterial () {

	return *this;
}

//----------------------------------------------------------------//
MOAIAbstractMaterial* MOAIMaterialMgr::MOAIAbstractMaterialInterface_GetMaterial () {

	return this;
}
