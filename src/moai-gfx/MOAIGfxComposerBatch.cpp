// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxComposerRetained.h>
#include <moai-gfx/MOAIGfxComposerBatch.h>

//================================================================//
// MOAIGfxComposerBatch
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxComposerBatch::Clear () {

	this->mComposers.Clear ();
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch::MOAIGfxComposerBatch () :
	mIndexBatchSize ( 1 ) {
	
	RTTI_BEGIN ( MOAIGfxComposerBatch )
		RTTI_EXTEND ( MOAIAbstractGfxComposerBatchInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch::~MOAIGfxComposerBatch () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIGfxComposerBatch::Reserve ( ZLSize n ) {

	// TODO: this was ZGL_FIRST_FLAG; unclear why
	assert ( n < 0x70000000 ); // probably don't need more than 0x70000000 materials...

	this->Clear ();
	this->mComposers.Init ( n );
}

//----------------------------------------------------------------//
size_t MOAIGfxComposerBatch::Size () {

	return this->mComposers.Size ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxComposer& MOAIGfxComposerBatch::MOAIAbstractGfxComposerBatchInterface_AffirmComposer ( ZLIndex index ) {

	this->mComposers.Grow (( ZLSize )index + 1 );

	MOAIAbstractGfxComposer* composer = this->mComposers [ index ];
	if ( !composer ) {
		composer = new MOAIGfxComposerRetained ();
		this->mComposers [ index ] = composer;
	}
	return *composer;
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch& MOAIGfxComposerBatch::MOAIAbstractGfxComposerBatchInterface_AffirmComposerBatch () {

	return *this;
}

//----------------------------------------------------------------//
MOAIAbstractGfxComposer* MOAIGfxComposerBatch::MOAIAbstractGfxComposerBatchInterface_GetComposer ( ZLIndex index ) {

	return ( index < this->mComposers.Size ()) ? ( MOAIAbstractGfxComposer* )this->mComposers [ index ] : NULL;
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch* MOAIGfxComposerBatch::MOAIAbstractGfxComposerBatchInterface_GetComposerBatch () {

	return this;
}
