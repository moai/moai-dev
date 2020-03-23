// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxScript.h>
#include <moai-gfx/MOAIGfxScriptBatch.h>

//================================================================//
// MOAIGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::Clear () {

	this->mComposers.Clear ();
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch::MOAIGfxScriptBatch () :
	mIndexBatchSize ( 1 ) {
	
	RTTI_BEGIN ( MOAIGfxScriptBatch )
		RTTI_EXTEND ( MOAIAbstractGfxScriptBatchInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch::~MOAIGfxScriptBatch () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::Reserve ( ZLSize n ) {

	// TODO: this was ZGL_FIRST_FLAG; unclear why
	assert ( n < 0x70000000 ); // probably don't need more than 0x70000000 materials...

	this->Clear ();
	this->mComposers.Init ( n );
}

//----------------------------------------------------------------//
size_t MOAIGfxScriptBatch::Size () {

	return this->mComposers.Size ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxScript& MOAIGfxScriptBatch::MOAIAbstractGfxScriptBatchInterface_AffirmComposer ( ZLIndex index ) {

	this->mComposers.Grow (( ZLSize )index + 1 );

	MOAIAbstractGfxScript* composer = this->mComposers [ index ];
	if ( !composer ) {
		composer = new MOAIGfxScript ();
		this->mComposers [ index ] = composer;
	}
	return *composer;
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch& MOAIGfxScriptBatch::MOAIAbstractGfxScriptBatchInterface_AffirmComposerBatch () {

	return *this;
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIGfxScriptBatch::MOAIAbstractGfxScriptBatchInterface_GetComposer ( ZLIndex index ) {

	return ( index < this->mComposers.Size ()) ? ( MOAIAbstractGfxScript* )this->mComposers [ index ] : NULL;
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch* MOAIGfxScriptBatch::MOAIAbstractGfxScriptBatchInterface_GetComposerBatch () {

	return this;
}
