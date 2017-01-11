// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIMaterialStackMgr.h>
#include <moai-sim/MOAIShaderMgr.h>

//================================================================//
// MOAIMaterialStackMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterialStackMgr::Clear () {

	while ( this->mStack.GetTop () > 1 ) {
	
		// do it this way so the destructors get called
		this->mStack.Pop ();
	}
}

//----------------------------------------------------------------//
void MOAIMaterialStackMgr::LoadGfxState ( u32 defaultShader ) {

	this->mStack.Top ().LoadGfxState ( defaultShader );
}

//----------------------------------------------------------------//
void MOAIMaterialStackMgr::LoadGfxState ( MOAIShader* defaultShader ) {

	this->mStack.Top ().LoadGfxState ( defaultShader );
}

//----------------------------------------------------------------//
MOAIMaterialStackMgr::MOAIMaterialStackMgr () {
	
	this->mStack.Push ();
}

//----------------------------------------------------------------//
MOAIMaterialStackMgr::~MOAIMaterialStackMgr () {
}

//----------------------------------------------------------------//
void MOAIMaterialStackMgr::Pop () {

	if ( this->mStack.GetTop () > 1 ) {
		this->mStack.Pop ();
	}
}

//----------------------------------------------------------------//
void MOAIMaterialStackMgr::Push ( const MOAIMaterial* material ) {

	MOAIMaterial& top = this->mStack.Top ();
	MOAIMaterial& push = this->mStack.Push ( top );
	
	if ( material ) {
		push.Compose ( *material );
	}
}

//----------------------------------------------------------------//
const MOAIMaterial& MOAIMaterialStackMgr::Top () {

	return this->mStack.Top ();
}

//================================================================//
// MOAIScopedMaterialStack
//================================================================//

//----------------------------------------------------------------//
void MOAIScopedMaterialStack::Clear () {

	this->mMaterialStack.Clear ();
	this->mRestoreTop = this->mMaterialStack.mStack.GetTop ();
}

//----------------------------------------------------------------//
void MOAIScopedMaterialStack::LoadGfxState ( u32 defaultShader ) {

	this->mMaterialStack.LoadGfxState ( defaultShader );
}

//----------------------------------------------------------------//
void MOAIScopedMaterialStack::LoadGfxState ( MOAIShader* defaultShader ) {

	this->mMaterialStack.LoadGfxState ( defaultShader );
}

//----------------------------------------------------------------//
MOAIScopedMaterialStack::MOAIScopedMaterialStack () :
	mMaterialStack ( MOAIMaterialStackMgr::Get ()) {
	
	this->mRestoreTop = this->mMaterialStack.mStack.GetTop ();
}

//----------------------------------------------------------------//
MOAIScopedMaterialStack::~MOAIScopedMaterialStack () {

	u32 top = this->mMaterialStack.mStack.GetTop ();
	for ( ; top > this->mRestoreTop; --top ) {
		this->mMaterialStack.Pop ();
	}
}

//----------------------------------------------------------------//
void MOAIScopedMaterialStack::Pop () {

	this->mMaterialStack.Pop ();
}

//----------------------------------------------------------------//
void MOAIScopedMaterialStack::Push ( const MOAIMaterial* material ) {

	this->mMaterialStack.Push ( material );
}

//----------------------------------------------------------------//
const MOAIMaterial& MOAIScopedMaterialStack::Top () {

	return this->mMaterialStack.Top ();
}
