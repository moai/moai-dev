// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDeviceBase.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIGfxDeviceBase
//================================================================//

//----------------------------------------------------------------//
MOAIGfxDeviceBase::MOAIGfxDeviceBase () :
	mShaderDirty ( false ),
	mDrawCount ( 0 ),
	mCurrentIdxBuffer ( 0 ),
	mCurrentVtxBuffer ( 0 ),
	mCurrentVtxFormat ( 0 ) {
	
	this->mViewRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIGfxDeviceBase::~MOAIGfxDeviceBase () {
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::SetIndexBuffer ( MOAIIndexBuffer* buffer ) {

	if ( this->mCurrentIdxBuffer != buffer ) {
	
		if ( this->mCurrentIdxBuffer ) {
			this->mCurrentIdxBuffer->Unbind ();
		}
	
		this->mCurrentIdxBuffer = buffer;
		
		if ( buffer ) {
			buffer->Bind ();
		}
	}
	else if ( this->mCurrentIdxBuffer && this->mCurrentIdxBuffer->NeedsFlush ()) {
		
		this->mCurrentIdxBuffer->Bind ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::SetVertexBuffer ( MOAIVertexBuffer* buffer ) {

	if ( this->mCurrentVtxBuffer != buffer ) {
	
		this->FlushBufferedPrims ();
		
		if ( this->mCurrentVtxBuffer ) {
			this->mCurrentVtxBuffer->Unbind ();
		}
		
		this->mCurrentVtxBuffer = buffer;
		this->mCurrentVtxFormat = 0;
	
		if ( buffer ) {
			buffer->Bind ();
		}
	}
	else if ( this->mCurrentVtxBuffer && this->mCurrentVtxBuffer->NeedsFlush ()) {
		
		this->mCurrentVtxBuffer->Bind ();
		this->mCurrentVtxFormat = 0;
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::SetVertexFormat ( MOAIVertexFormat* format ) {

	assert ( this->mCurrentVtxBuffer ); // must currently have a valid vertex buffer bound (to receive the vertex format)
	
	if ( this->mCurrentVtxFormat != format ) {
	
		this->FlushBufferedPrims ();
	
		if ( this->mCurrentVtxFormat ) {
			this->mCurrentVtxFormat->Unbind ();
		}
		
		this->mCurrentVtxFormat = format;
			
		if ( format ) {
			format->Bind ();
		}
	}
}
