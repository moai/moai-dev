// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfx.h>
#include <zl-gfx/ZLGfxMgrGL.h>
#include <zl-gfx/ZLVertexArrayGL.h>
#include <zl-gfx/ZLVertexBufferGL.h>
#include <zl-gfx/ZLVertexFormatGL.h>

//================================================================//
// ZLVertexBufferWithFormatGL
//================================================================//

//----------------------------------------------------------------//
ZLVertexBufferWithFormatGL::ZLVertexBufferWithFormatGL () {
}


//----------------------------------------------------------------//
ZLVertexBufferWithFormatGL::~ZLVertexBufferWithFormatGL () {
}

//----------------------------------------------------------------//
void ZLVertexBufferWithFormatGL::SetBufferAndFormat ( ZLVertexBufferGL* buffer, ZLVertexFormatGL* format ) {

	this->mBuffer = buffer;
	this->mFormat = format;
}

//================================================================//
// ZLVertexArrayGL
//================================================================//

//----------------------------------------------------------------//
bool ZLVertexArrayGL::AffirmVertexBuffers ( u32 idx ) {

	if (( this->mVertexBuffers.Size () == 0 ) && ( idx == 0 )) {
		this->ReserveVertexBuffers ( 1 );
	}
	return ( idx < this->mVertexBuffers.Size ());
}

//----------------------------------------------------------------//
void ZLVertexArrayGL::BindVertexArrayItems () {

	ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();

	ZLSize totalVBOs = this->mVertexBuffers.Size ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVBOs; ++i ) {
	
		ZLVertexBufferGL* buffer = this->mVertexBuffers [ i ].mBuffer;
		ZLVertexFormatGL* format = this->mVertexBuffers [ i ].mFormat;
		
		assert ( buffer && format );
		
		assert (( this->mUseVAOs && buffer->IsUsingVBOs ()) || ( !this->mUseVAOs )); // buffer objects must use VBOs to work with VAOs
		
		ZLSharedConstBuffer* bufferForBind = buffer->GetBufferForBind ( gfx );
		
		buffer->Bind ();
		format->Bind ( gfx, bufferForBind );
		buffer->Unbind ();
	}
}

//----------------------------------------------------------------//
ZLVertexBufferGL* ZLVertexArrayGL::GetVertexBuffer ( ZLIndex idx ) {

	return idx < this->mVertexBuffers.Size () ? ( ZLVertexBufferGL* )this->mVertexBuffers [ idx ].mBuffer : ( ZLVertexBufferGL* )0;
}

//----------------------------------------------------------------//
ZLVertexFormatGL* ZLVertexArrayGL::GetVertexFormat ( ZLIndex idx ) {

	return idx < this->mVertexBuffers.Size () ? ( ZLVertexFormatGL* )this->mVertexBuffers [ idx ].mFormat : ( ZLVertexFormatGL* )0;
}

//----------------------------------------------------------------//
void ZLVertexArrayGL::ReserveVAOs ( u32 total ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVAOs.Size (); ++i ) {
		this->mGfxMgr->DeleteOrDiscard ( this->mVAOs [ i ], false );
	}
	this->mVAOs.Init ( total );
	this->FinishInit ();
}

//----------------------------------------------------------------//
void ZLVertexArrayGL::ReserveVertexBuffers ( u32 total ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVertexBuffers.Size (); ++i ) {
		this->mVertexBuffers [ i ].SetBufferAndFormat ( 0, 0 );
	}
	this->mVertexBuffers.Init ( total );
	
	this->FinishInit ();
}

//----------------------------------------------------------------//
void ZLVertexArrayGL::SetVertexBuffer ( ZLIndex idx, ZLVertexBufferGL* vtxBuffer, ZLVertexFormatGL* vtxFormat ) {

	if ( this->AffirmVertexBuffers ( idx )) {
		this->mVertexBuffers [ idx ].SetBufferAndFormat ( vtxBuffer, vtxFormat );
		this->ScheduleForGPUUpdate ();
	}
}

//----------------------------------------------------------------//
void ZLVertexArrayGL::UnbindVertexArrayItems () {

	size_t totalVBOs = this->mVertexBuffers.Size ();
	for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVBOs; ++i ) {
	
		ZLVertexFormatGL* format = this->mVertexBuffers [ i ].mFormat;
		assert ( format );
		format->Unbind ( this->mGfxMgr->GetDrawingAPI ());
	}
}

//----------------------------------------------------------------//
ZLVertexArrayGL::ZLVertexArrayGL () :
	mCurrentVAO ( ZLIndexOp::INVALID ),
	mUseVAOs ( false ) {
}

//----------------------------------------------------------------//
ZLVertexArrayGL::~ZLVertexArrayGL () {

	this->ReserveVAOs ( 0 );
	this->ReserveVertexBuffers ( 0 );
}


//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool ZLVertexArrayGL::ZLAbstractGfxResource_OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void ZLVertexArrayGL::ZLAbstractGfxResource_OnCPUDestroy () {
}

//----------------------------------------------------------------//
void ZLVertexArrayGL::ZLAbstractGfxResource_OnGPUBind () {

	if ( this->mUseVAOs && this->mVAOs.Size ()) {
		this->mGfxMgr->GetDrawingAPI ().BindVertexArray ( this->mVAOs [ this->mCurrentVAO ]);
	}
	else {
		this->BindVertexArrayItems ();
	}
}

//----------------------------------------------------------------//
bool ZLVertexArrayGL::ZLAbstractGfxResource_OnGPUCreate () {

	this->mUseVAOs = false;
	
	size_t totalVAOs = this->mVAOs.Size ();

	if ( totalVAOs ) {
		
		for ( ZLIndex i = ZLIndexOp::ZERO; i < totalVAOs; ++i ) {
			ZLGfxHandleGL vao = this->mGfxMgr->GetDrawingAPI ().CreateVertexArray (); // OK for this to return 0
			if ( vao.GetType () != ZLGfxResourceGL::NONE ) {
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
void ZLVertexArrayGL::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVAOs.Size (); ++i ) {
		this->mGfxMgr->DeleteOrDiscard ( this->mVAOs [ i ], shouldDelete );
	}
}

//----------------------------------------------------------------//
void ZLVertexArrayGL::ZLAbstractGfxResource_OnGPUUnbind () {

	if ( this->mUseVAOs ) {
		this->mGfxMgr->GetDrawingAPI ().BindVertexArray ( ZLGfxResourceGL::UNBIND );
	}
	else {
		this->UnbindVertexArrayItems ();
	}
}

//----------------------------------------------------------------//
bool ZLVertexArrayGL::ZLAbstractGfxResource_OnGPUUpdate () {

	if ( !this->mUseVAOs ) return true;
	if ( !this->mVAOs.Size ()) return false;

	this->mCurrentVAO = ZLIndexOp::AddAndWrap ( this->mCurrentVAO, 1, this->mVAOs.Size ());
	const ZLGfxHandleGL& vao = this->mVAOs [ this->mCurrentVAO ];
	
	if ( vao.CanBind ()) {
		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
		gfx.BindVertexArray ( vao );
		this->BindVertexArrayItems ();
		gfx.BindVertexArray ( ZLGfxResourceGL::UNBIND );
		return true;
	}
	return false;
}
