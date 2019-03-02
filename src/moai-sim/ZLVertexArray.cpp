// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/ZLGfxResourceClerk.h>
#include <moai-sim/ZLVertexArray.h>

//================================================================//
// ZLVertexBufferWithFormat
//================================================================//

//----------------------------------------------------------------//
ZLVertexBufferWithFormat::ZLVertexBufferWithFormat () {
}


//----------------------------------------------------------------//
ZLVertexBufferWithFormat::~ZLVertexBufferWithFormat () {
}

//----------------------------------------------------------------//
void ZLVertexBufferWithFormat::SetBufferAndFormat ( ZLVertexBuffer* buffer, MOAIVertexFormat* format ) {

	this->mBuffer = buffer;
	this->mFormat = format;
}

//================================================================//
// ZLVertexArray
//================================================================//

//----------------------------------------------------------------//
bool ZLVertexArray::AffirmVertexBuffers ( u32 idx ) {

	if (( this->mVertexBuffers.Size () == 0 ) && ( idx == 0 )) {
		this->ReserveVertexBuffers ( 1 );
	}
	return ( idx < this->mVertexBuffers.Size ());
}

//----------------------------------------------------------------//
void ZLVertexArray::BindVertexArrayItems () {

	ZLGfxStateCache& gfxState = MOAIGfxMgr::Get ().mGfxState;

	ZLSize totalVBOs = this->mVertexBuffers.Size ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVBOs; ++i ) {
	
		ZLVertexBuffer* buffer = this->mVertexBuffers [ i ].mBuffer;
		MOAIVertexFormat* format = this->mVertexBuffers [ i ].mFormat;
	
		assert ( buffer && format );
		
		assert (( this->mUseVAOs && buffer->IsUsingVBOs ()) || ( !this->mUseVAOs )); // buffer objects must use VBOs to work with VAOs
	
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
		ZLSharedConstBuffer* bufferForBind = buffer->GetBufferForBind ( gfx );
		
		buffer->Bind ();
		format->Bind ( bufferForBind );
		buffer->Unbind ();
	}
}

//----------------------------------------------------------------//
ZLVertexBuffer* ZLVertexArray::GetVertexBuffer ( ZLIndex idx ) {

	return idx < this->mVertexBuffers.Size () ? ( ZLVertexBuffer* )this->mVertexBuffers [ idx ].mBuffer : ( ZLVertexBuffer* )0;
}

//----------------------------------------------------------------//
MOAIVertexFormat* ZLVertexArray::GetVertexFormat ( ZLIndex idx ) {

	return idx < this->mVertexBuffers.Size () ? ( MOAIVertexFormat* )this->mVertexBuffers [ idx ].mFormat : ( MOAIVertexFormat* )0;
}

//----------------------------------------------------------------//
void ZLVertexArray::ReserveVAOs ( u32 total ) {

	if ( MOAIGfxMgr::IsValid ()) {
		for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVAOs.Size (); ++i ) {
			ZLGfxResourceClerk::DeleteOrDiscard ( this->mVAOs [ i ], false );
		}
	}
	this->mVAOs.Init ( total );
	
	this->FinishInit ();
}

//----------------------------------------------------------------//
void ZLVertexArray::ReserveVertexBuffers ( u32 total ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVertexBuffers.Size (); ++i ) {
		this->mVertexBuffers [ i ].SetBufferAndFormat ( 0, 0 );
	}
	this->mVertexBuffers.Init ( total );
	
	this->FinishInit ();
}

//----------------------------------------------------------------//
void ZLVertexArray::SetVertexBuffer ( ZLIndex idx, ZLVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) {

	if ( this->AffirmVertexBuffers ( idx )) {
		this->mVertexBuffers [ idx ].SetBufferAndFormat ( vtxBuffer, vtxFormat );
		this->ScheduleForGPUUpdate ();
	}
}

//----------------------------------------------------------------//
void ZLVertexArray::UnbindVertexArrayItems () {

	ZLGfxStateCache& gfxState = MOAIGfxMgr::Get ().mGfxState;

	size_t totalVBOs = this->mVertexBuffers.Size ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVBOs; ++i ) {
	
		MOAIVertexFormat* format = this->mVertexBuffers [ i ].mFormat;
		assert ( format );
		format->Unbind ();
	}
}

//----------------------------------------------------------------//
ZLVertexArray::ZLVertexArray () :
	mCurrentVAO ( ZLIndexOp::INVALID ),
	mUseVAOs ( false ) {
}

//----------------------------------------------------------------//
ZLVertexArray::~ZLVertexArray () {

	this->ReserveVAOs ( 0 );
	this->ReserveVertexBuffers ( 0 );
}


//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool ZLVertexArray::ZLAbstractGfxResource_OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void ZLVertexArray::ZLAbstractGfxResource_OnCPUDestroy () {
}

//----------------------------------------------------------------//
void ZLVertexArray::ZLAbstractGfxResource_OnGPUBind () {

	if ( this->mUseVAOs && this->mVAOs.Size ()) {
		MOAIGfxMgr::GetDrawingAPI ().BindVertexArray ( this->mVAOs [ this->mCurrentVAO ]);
	}
	else {
		this->BindVertexArrayItems ();
	}
}

//----------------------------------------------------------------//
bool ZLVertexArray::ZLAbstractGfxResource_OnGPUCreate () {

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	this->mUseVAOs = false;
	
	size_t totalVAOs = this->mVAOs.Size ();

	if ( totalVAOs ) {
		
		for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVAOs; ++i ) {
			ZLGfxHandle vao = gfx.CreateVertexArray (); // OK for this to return 0
			if ( vao.GetType () != ZLGfxResource::NONE ) {
				this->mVAOs [ i ] = vao;
				this->mUseVAOs = true;
			}
		}
	}
	
	this->mCurrentVAO = ZLIndexCast ( 0 );
	this->ZLAbstractGfxResource_OnGPUUpdate ();
	
	return true;
}

//----------------------------------------------------------------//
void ZLVertexArray::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVAOs.Size (); ++i ) {
		ZLGfxResourceClerk::DeleteOrDiscard ( this->mVAOs [ i ], shouldDelete );
	}
}

//----------------------------------------------------------------//
void ZLVertexArray::ZLAbstractGfxResource_OnGPUUnbind () {

	if ( this->mUseVAOs ) {
		MOAIGfxMgr::GetDrawingAPI ().BindVertexArray ( ZLGfxResource::UNBIND );
	}
	else {
		this->UnbindVertexArrayItems ();
	}
}

//----------------------------------------------------------------//
bool ZLVertexArray::ZLAbstractGfxResource_OnGPUUpdate () {

	if ( !this->mUseVAOs ) return true;
	if ( !this->mVAOs.Size ()) return false;

	this->mCurrentVAO =  ZLIndexOp::AddAndWrap ( this->mCurrentVAO, 1, this->mVAOs.Size ());
	const ZLGfxHandle& vao = this->mVAOs [ this->mCurrentVAO ];
	
	if ( vao.CanBind ()) {
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		gfx.BindVertexArray ( vao );
		this->BindVertexArrayItems ();
		gfx.BindVertexArray ( ZLGfxResource::UNBIND );
		return true;
	}
	return false;
}
