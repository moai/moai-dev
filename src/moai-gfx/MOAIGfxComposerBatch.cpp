// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxComposer.h>
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
		RTTI_EXTEND ( MOAIGfxComposerBatchInterface )
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
MOAIGfxComposer& MOAIGfxComposerBatch::MOAIGfxComposerBatchInterface_AffirmComposer ( ZLIndex index ) {

	this->mComposers.Grow (( ZLSize )index + 1 );

	MOAIGfxComposer* composer = this->mComposers [ index ];
	if ( !composer ) {
		composer = new MOAIGfxComposer ();
		this->mComposers [ index ] = composer;
	}
	return *composer;
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch& MOAIGfxComposerBatch::MOAIGfxComposerBatchInterface_AffirmComposerBatch () {

	return *this;
}

//----------------------------------------------------------------//
MOAIGfxComposer* MOAIGfxComposerBatch::MOAIGfxComposerBatchInterface_GetComposer ( ZLIndex index ) {

	return ( index < this->mComposers.Size ()) ? ( MOAIGfxComposer* )this->mComposers [ index ] : NULL;
}

//----------------------------------------------------------------//
MOAIGfxComposerBatch* MOAIGfxComposerBatch::MOAIGfxComposerBatchInterface_GetComposerBatch () {

	return this;
}
