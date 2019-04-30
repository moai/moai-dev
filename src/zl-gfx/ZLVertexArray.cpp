// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfx.h>
#include <zl-gfx/ZLGfxMgrGL.h>
#include <zl-gfx/ZLVertexArray.h>
#include <zl-gfx/ZLVertexBuffer.h>
#include <zl-gfx/ZLVertexFormat.h>

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
void ZLVertexBufferWithFormat::SetBufferAndFormat ( ZLVertexBuffer* buffer, ZLVertexFormat* format ) {

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

	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

	ZLSize totalVBOs = this->mVertexBuffers.Size ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVBOs; ++i ) {
	
		ZLVertexBuffer* buffer = this->mVertexBuffers [ i ].mBuffer;
		ZLVertexFormat* format = this->mVertexBuffers [ i ].mFormat;
		
		assert ( buffer && format );
		
		assert (( this->mUseVAOs && buffer->IsUsingVBOs ()) || ( !this->mUseVAOs )); // buffer objects must use VBOs to work with VAOs
		
		ZLSharedConstBuffer* bufferForBind = buffer->GetBufferForBind ( gfx );
		
		buffer->Bind ();
		format->Bind ( gfx, bufferForBind );
		buffer->Unbind ();
	}
}

//----------------------------------------------------------------//
ZLVertexBuffer* ZLVertexArray::GetVertexBuffer ( ZLIndex idx ) {

	return idx < this->mVertexBuffers.Size () ? ( ZLVertexBuffer* )this->mVertexBuffers [ idx ].mBuffer : ( ZLVertexBuffer* )0;
}

//----------------------------------------------------------------//
ZLVertexFormat* ZLVertexArray::GetVertexFormat ( ZLIndex idx ) {

	return idx < this->mVertexBuffers.Size () ? ( ZLVertexFormat* )this->mVertexBuffers [ idx ].mFormat : ( ZLVertexFormat* )0;
}

//----------------------------------------------------------------//
void ZLVertexArray::ReserveVAOs ( u32 total ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVAOs.Size (); ++i ) {
		this->mGfxMgr->DeleteOrDiscard ( this->mVAOs [ i ], false );
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
void ZLVertexArray::SetVertexBuffer ( ZLIndex idx, ZLVertexBuffer* vtxBuffer, ZLVertexFormat* vtxFormat ) {

	if ( this->AffirmVertexBuffers ( idx )) {
		this->mVertexBuffers [ idx ].SetBufferAndFormat ( vtxBuffer, vtxFormat );
		this->ScheduleForGPUUpdate ();
	}
}

//----------------------------------------------------------------//
void ZLVertexArray::UnbindVertexArrayItems () {

	size_t totalVBOs = this->mVertexBuffers.Size ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVBOs; ++i ) {
	
		ZLVertexFormat* format = this->mVertexBuffers [ i ].mFormat;
		assert ( format );
		format->Unbind ( this->mGfxMgr->GetDrawingAPI ());
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
		this->mGfxMgr->GetDrawingAPI ().BindVertexArray ( this->mVAOs [ this->mCurrentVAO ]);
	}
	else {
		this->BindVertexArrayItems ();
	}
}

//----------------------------------------------------------------//
bool ZLVertexArray::ZLAbstractGfxResource_OnGPUCreate () {

	this->mUseVAOs = false;
	
	size_t totalVAOs = this->mVAOs.Size ();

	if ( totalVAOs ) {
		
		for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVAOs; ++i ) {
			ZLGfxHandle vao = this->mGfxMgr->GetDrawingAPI ().CreateVertexArray (); // OK for this to return 0
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
		this->mGfxMgr->DeleteOrDiscard ( this->mVAOs [ i ], shouldDelete );
	}
}

//----------------------------------------------------------------//
void ZLVertexArray::ZLAbstractGfxResource_OnGPUUnbind () {

	if ( this->mUseVAOs ) {
		this->mGfxMgr->GetDrawingAPI ().BindVertexArray ( ZLGfxResource::UNBIND );
	}
	else {
		this->UnbindVertexArrayItems ();
	}
}

//----------------------------------------------------------------//
bool ZLVertexArray::ZLAbstractGfxResource_OnGPUUpdate () {

	if ( !this->mUseVAOs ) return true;
	if ( !this->mVAOs.Size ()) return false;

	this->mCurrentVAO = ZLIndexOp::AddAndWrap ( this->mCurrentVAO, 1, this->mVAOs.Size ());
	const ZLGfxHandle& vao = this->mVAOs [ this->mCurrentVAO ];
	
	if ( vao.CanBind ()) {
		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
		gfx.BindVertexArray ( vao );
		this->BindVertexArrayItems ();
		gfx.BindVertexArray ( ZLGfxResource::UNBIND );
		return true;
	}
	return false;
}
