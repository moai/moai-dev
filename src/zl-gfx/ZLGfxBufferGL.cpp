// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfx.h>
#include <zl-gfx/ZLGfxBufferGL.h>
#include <zl-gfx/ZLGfxEnum.h>
#include <zl-gfx/ZLGfxMgrGL.h>
#include <zl-gfx/ZLGfxResourceClerkGL.h>

//================================================================//
// ZLGfxBufferGL
//================================================================//

//----------------------------------------------------------------//
void ZLGfxBufferGL::Clear () {

	this->ZLCopyOnWrite::Free ();

	this->mVBOs.Clear ();
	this->mCurrentVBO = ZLIndexOp::ZERO;
	
	this->Destroy ();
}

//----------------------------------------------------------------//
void ZLGfxBufferGL::CopyFromStream ( ZLStream& stream, size_t size ) {

	this->Reserve (( u32 )size );
	this->WriteStream ( stream );
	
	this->ScheduleForGPUUpdate ();
}

//----------------------------------------------------------------//
size_t ZLGfxBufferGL::CountVBOs () {

	return this->mVBOs.Size ();
}

//----------------------------------------------------------------//
ZLSharedConstBuffer* ZLGfxBufferGL::GetBufferForBind ( ZLGfx& gfx ) {
	UNUSED(gfx);
	return this->mUseVBOs ? 0 : this->ZLCopyOnWrite::GetSharedConstBuffer ();

//	if ( this->mUseVBOs ) return 0;
//
//	ZLSharedConstBuffer* buffer = this->ZLCopyOnWrite::GetSharedConstBuffer ();
//	return this->mCopyOnBind ? gfx.CopyBuffer ( buffer ) : buffer;
}

//----------------------------------------------------------------//
void ZLGfxBufferGL::Reserve ( ZLSize size ) {
	
	this->ZLCopyOnWrite::Free ();
	
	if ( size ) {
		this->ZLCopyOnWrite::Reserve ( size );
		this->FinishInit ();
	}
}

//----------------------------------------------------------------//
void ZLGfxBufferGL::ReserveVBOs ( ZLSize gpuBuffers ) {

	if ( gpuBuffers < this->mVBOs.Size ()) {
		this->mVBOs.Clear ();
	}

	if ( gpuBuffers ) {
		this->mVBOs.Resize ( gpuBuffers );
		this->mCurrentVBO = ZLIndexCast ( gpuBuffers - 1 );
	}

	this->FinishInit ();
}

//----------------------------------------------------------------//
ZLGfxBufferGL::ZLGfxBufferGL () :
	mCurrentVBO ( ZLIndexOp::ZERO ),
	mTarget ( ZGL_BUFFER_TARGET_ARRAY ),
	mUseVBOs ( false ),
	mCopyOnUpdate ( false ) {
	
	RTTI_SINGLE ( ZLAbstractGfxResourceGL )
}

//----------------------------------------------------------------//
ZLGfxBufferGL::~ZLGfxBufferGL () {

	this->Clear ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool ZLGfxBufferGL::ZLAbstractGfxResource_OnCPUCreate () {
	return true;
}

//----------------------------------------------------------------//
void ZLGfxBufferGL::ZLAbstractGfxResource_OnCPUDestroy () {
}

//----------------------------------------------------------------//
void ZLGfxBufferGL::ZLAbstractGfxResource_OnGPUBind () {
	
	if ( !this->mUseVBOs ) return;
	
	const ZLGfxHandleGL& vbo = this->mVBOs [ this->mCurrentVBO ];
	
	if ( vbo.CanBind ()) {
		this->mGfxMgr->GetDrawingAPI ().BindBuffer ( this->mTarget, vbo );
	}
}

//----------------------------------------------------------------//
bool ZLGfxBufferGL::ZLAbstractGfxResource_OnGPUCreate () {

	u32 count = 0;

	this->mUseVBOs = ( this->mVBOs.Size () > 0 );
	
	if ( this->mUseVBOs ) {

		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
		u32 hint = this->mVBOs.Size () > 1 ? ZGL_BUFFER_USAGE_STREAM_DRAW : ZGL_BUFFER_USAGE_STATIC_DRAW;

		for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVBOs.Size (); ++i ) {
			
			ZLGfxHandleGL vbo = gfx.CreateBuffer ();
			
			// TODO: error handling
			//if ( vbo ) {
			
				ZLSharedConstBuffer* buffer = this->GetCursor () ? this->GetSharedConstBuffer () : 0;
			
//				if ( this->mCopyOnUpdate ) {
//					buffer = gfx.CopyBuffer ( buffer );
//				}
			
				gfx.BindBuffer ( this->mTarget, vbo );
				gfx.BufferData ( this->mTarget, this->GetLength (), buffer, 0, hint );
				gfx.BindBuffer ( this->mTarget, ZLGfxResourceGL::UNBIND );
			
				count++;
			//}
			this->mVBOs [ i ] = vbo;
		}
	}
	
	return count == this->mVBOs.Size ();
}

//----------------------------------------------------------------//
void ZLGfxBufferGL::ZLAbstractGfxResource_OnGPUDeleteOrDiscard ( bool shouldDelete ) {

	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mVBOs.Size (); ++i ) {
		this->mGfxMgr->DeleteOrDiscard ( this->mVBOs [ i ], shouldDelete );
	}
}

//----------------------------------------------------------------//
void ZLGfxBufferGL::ZLAbstractGfxResource_OnGPUUnbind () {

	this->mGfxMgr->GetDrawingAPI ().BindBuffer ( this->mTarget, ZLGfxResourceGL::UNBIND ); // OK?
}

//----------------------------------------------------------------//
bool ZLGfxBufferGL::ZLAbstractGfxResource_OnGPUUpdate () {

	if ( !this->mUseVBOs ) return true;
	
	bool dirty = this->GetCursor () > 0;
	
	if ( dirty ) {
		this->mCurrentVBO =  ZLIndexOp::AddAndWrap ( this->mCurrentVBO, 1, this->mVBOs.Size ());
	}
	
	const ZLGfxHandleGL& vbo = this->mVBOs [ this->mCurrentVBO ];
	
	if ( dirty && vbo.CanBind ()) {
		
		ZLGfx& gfx = this->mGfxMgr->GetDrawingAPI ();
		
		// TODO: There are a few different ways to approach updating buffers with varying performance
		// on different platforms. The approach here is just to multi-buffer the VBO and replace its
		// contents via zglBufferSubData when they change. The TODO here is to do performance tests
		// on multiple devices, evaluate other approaches and possible expose the configuration of
		// those to the end user via Lua.
			
		ZLSharedConstBuffer* buffer = this->GetSharedConstBuffer ();
		
//		if ( this->mCopyOnUpdate ) {
//			buffer = gfx.CopyBuffer ( buffer );
//		}
		
		ZLGfxHandleGL vbo = gfx.CreateBuffer ();
		gfx.BindBuffer ( this->mTarget, vbo );
		gfx.BufferSubData ( this->mTarget, 0, this->GetCursor (), buffer, 0 );
		gfx.BindBuffer ( this->mTarget, ZLGfxResourceGL::UNBIND );
	
		//u32 hint = this->mVBOs.Size () > 1 ? ZGL_BUFFER_USAGE_DYNAMIC_DRAW : ZGL_BUFFER_USAGE_STATIC_DRAW;
		//zglBufferData ( this->mTarget, this->GetLength (), 0, hint );
	}
	
	return true;
}
