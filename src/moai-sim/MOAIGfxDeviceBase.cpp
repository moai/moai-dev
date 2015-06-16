// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxDeviceBase.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIVertexArray.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIGfxDeviceBase
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::BindVertexArray ( MOAIVertexArray* vtxArray ) {

	if ( this->mCurrentVtxArray != vtxArray ) {
	
		if ( this->mCurrentVtxArray ) {
			this->mCurrentVtxArray->Unbind ();
		}
		
		this->BindVertexBuffer ();
		this->mCurrentVtxArray = vtxArray;
		
		if ( vtxArray ) {
			vtxArray->Bind ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceBase::BindIndexBuffer ( MOAIIndexBuffer* buffer ) {

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
void MOAIGfxDeviceBase::BindVertexBuffer ( MOAIVertexBuffer* buffer ) {

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
void MOAIGfxDeviceBase::BindVertexFormat ( MOAIVertexFormat* format ) {

	if ( this->mCurrentVtxFormat != format ) {
	
		this->FlushBufferedPrims ();
	
		if ( this->mCurrentVtxFormat ) {
			this->mCurrentVtxFormat->Unbind ();
		}
		
		this->mCurrentVtxFormat = format;
			
		if ( format ) {
		
			assert ( this->mCurrentVtxBuffer ); // must currently have a valid vertex buffer bound (to receive the vertex format)
		
			format->Bind ();
		}
	}
}

//----------------------------------------------------------------//
MOAIGfxDeviceBase::MOAIGfxDeviceBase () :
	mShaderDirty ( false ),
	mDrawCount ( 0 ),
	mCurrentIdxBuffer ( 0 ),
	mCurrentVtxArray ( 0 ),
	mCurrentVtxBuffer ( 0 ),
	mCurrentVtxFormat ( 0 ) {
	
	this->mViewRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIGfxDeviceBase::~MOAIGfxDeviceBase () {
}
