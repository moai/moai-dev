// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxScriptRetained.h>
#include <moai-gfx/MOAIGfxScriptBatch.h>

//================================================================//
// MOAIGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptBatch::MOAIGfxScriptBatch () :
	mIndexBatchSize ( 1 ) {
	
	RTTI_BEGIN ( MOAIGfxScriptBatch )
		RTTI_EXTEND ( MOAIAbstractGfxScriptBatch )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxScriptBatch::~MOAIGfxScriptBatch () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIGfxScriptRetained& MOAIGfxScriptBatch::MOAIAbstractGfxScriptBatch_AffirmGfxScript ( ZLIndex index ) {

	this->mGfxScripts.Grow (( ZLSize )index + 1 );

	MOAIAbstractGfxScript* abstractGfxScript = this->mGfxScripts [ index ];
	MOAIGfxScriptRetained* gfxScript = abstractGfxScript ? MOAICast < MOAIGfxScriptRetained >( abstractGfxScript ) : NULL;
	
	if ( !gfxScript ) {
		gfxScript = new MOAIGfxScriptRetained ();
		this->mGfxScripts [ index ] = gfxScript;
	}
	return *gfxScript;
}

//----------------------------------------------------------------//
MOAIAbstractGfxScript* MOAIGfxScriptBatch::MOAIAbstractGfxScriptBatch_GetGfxScript ( ZLIndex index ) {

	return ( index < this->mGfxScripts.Size ()) ? ( MOAIAbstractGfxScript* )this->mGfxScripts [ index ] : NULL;
}

//----------------------------------------------------------------//
ZLSize MOAIGfxScriptBatch::MOAIAbstractGfxScriptBatch_GetIndexBatchSize () {

	return this->mIndexBatchSize;
}

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::MOAIAbstractGfxScriptBatch_ReserveGfxScripts ( ZLSize size ) {

	this->mGfxScripts.Init ( size );
}

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::MOAIAbstractGfxScriptBatch_SetGfxScript ( ZLIndex index, MOAIAbstractGfxScript* gfxScript ) {

	this->mGfxScripts.Grow ( index + 1 );
	this->mGfxScripts [ index ] = gfxScript;
}

//----------------------------------------------------------------//
void MOAIGfxScriptBatch::MOAIAbstractGfxScriptBatch_SetIndexBatchSize ( ZLSize size ) {

	this->mIndexBatchSize = size;
}
