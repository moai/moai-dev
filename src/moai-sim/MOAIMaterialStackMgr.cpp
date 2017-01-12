// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIMaterialStackMgr.h>
#include <moai-sim/MOAIShaderMgr.h>

//================================================================//
// MOAIMaterialStackScope
//================================================================//

//----------------------------------------------------------------//
MOAIMaterialStackScope::MOAIMaterialStackScope () :
	mMaterialStack ( MOAIMaterialStackMgr::Get ()) {
	
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
// MOAIMaterialStackMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIMaterialStackMgr::Clear () {

	this->mStack.Reset ();
	this->MOAIMaterial::Clear ();
}

//----------------------------------------------------------------//
MOAIMaterialStackMgr::MOAIMaterialStackMgr () {
}

//----------------------------------------------------------------//
MOAIMaterialStackMgr::~MOAIMaterialStackMgr () {
}

//----------------------------------------------------------------//
void MOAIMaterialStackMgr::Pop () {

	if ( this->mStack.GetTop () > 1 ) {
		u32 flags = this->mStack.Pop ();
		this->MOAIMaterial::Clear ( ~flags );
	}
	else {
		this->mStack.Reset ();
		this->MOAIMaterial::Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIMaterialStackMgr::Push ( const MOAIMaterial* material ) {
	
	this->mStack.Push ( this->mFlags );
	
	if ( material ) {
		this->Compose ( *material );
	}
}
