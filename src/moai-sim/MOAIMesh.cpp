// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIVertexFormat.h>

//----------------------------------------------------------------//
void MOAIVertexBuffer::ClearBounds () {

	this->mHasBounds = false;
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIVertexBuffer::MOAIVertexBuffer () :
	mTotalElements ( 0 ),
	mHasBounds ( false ) {

	this->ClearBounds ();
}

//----------------------------------------------------------------//
MOAIVertexBuffer::~MOAIVertexBuffer () {
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::SetBufferAndFormat ( MOAIMesh& owner, MOAIGfxBuffer* buffer, MOAIVertexFormat* format ) {

	this->mBuffer.Set ( owner, buffer );
	this->mFormat.Set ( owner, format );
	
	this->ClearBounds ();
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_reserveVAOs ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->ReserveVAOs ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_reserveVertexBuffers ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->ReserveVertexBuffers ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setIndexBuffer
	@text	Set the index buffer to render.
	
	@in		MOAIMesh self
	@in		MOAIIndexBuffer indexBuffer
	@out	nil
*/
int MOAIMesh::_setIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->SetIndexBuffer ( state.GetLuaObject < MOAIIndexBuffer >( 2, true ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPenWidth
	@text	Sets the pen with for drawing prims in this vertex buffer.
			Only valid with prim types GL_LINES, GL_LINE_LOOP, GL_LINE_STRIP.
	
	@in		MOAIMesh self
	@in		number penWidth
	@out	nil
*/
int MOAIMesh::_setPenWidth ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "UN" )
	
	self->mPenWidth = state.GetValue < float >( 2, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPointSize
	@text	Sets the point size for drawing prims in this vertex buffer.
			Only valid with prim types GL_POINTS.
	
	@in		MOAIMesh self
	@in		number pointSize
	@out	nil
*/
int MOAIMesh::_setPointSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "UN" )
	
	self->mPointSize = state.GetValue < float >( 2, 1.0f );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPrimType
	@text	Sets the prim type the buffer represents.
	
	@in		MOAIMesh self
	@in		number primType		One of MOAIMesh GL_POINTS, GL_LINES, GL_TRIANGLES, GL_LINE_LOOP,
								GL_LINE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP
	@out	nil
*/
int MOAIMesh::_setPrimType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "UN" )
	
	self->mPrimType = state.GetValue < u32 >( 2, 0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setVertexBuffer
	@text	Set the vertex buffer to render.
	
	@in		MOAIMesh self
	@in		MOAIGfxBuffer vertexBuffer
	@out	nil
*/
int MOAIMesh::_setVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	u32 baseParam = 2;
	u32 idx = 0;
	
	if ( state.IsType ( baseParam, LUA_TNUMBER )) {
		idx = state.GetValue < u32 >( baseParam++, 1 ) - 1;
	}
	
	MOAIGfxBuffer* buffer		= state.GetLuaObject < MOAIGfxBuffer >( baseParam++, false );
	MOAIVertexFormat* format	= state.GetLuaObject < MOAIVertexFormat >( baseParam++, false );
	
	self->SetVertexBuffer ( idx, buffer, format );
	self->DoCPUAffirm ();

	return 0;
}

//================================================================//
// MOAIMesh
//================================================================//

////----------------------------------------------------------------//
//void MOAIGfxBuffer::Bless () {
//}
//
////----------------------------------------------------------------//
//void MOAIGfxBuffer::Bless ( const MOAIVertexFormat& format ) {
//		
//	this->Seek ( 0, SEEK_SET );
//	this->mHasBounds = format.ComputeBounds ( this->mBounds, *this, this->GetSize ());
//	this->mTotalElements = ( u32 )( this->GetSize () / format.GetVertexSize ());
//	
//	this->DoCPUAffirm ();
//	this->MakeDirty ();
//}
//
////----------------------------------------------------------------//
//void MOAIGfxBuffer::Bless ( u32 offset, u32 stride, u32 components ) {
//	
//	this->Seek ( 0, SEEK_SET );
//	this->mHasBounds = MOAIVertexFormat::ComputeBounds ( this->mBounds, *this, this->GetSize (), offset, stride, components );
//	this->mTotalElements = ( u32 )( this->GetSize () / stride );
//
//	this->DoCPUAffirm ();
//	this->MakeDirty ();
//}

//----------------------------------------------------------------//
ZLBox MOAIMesh::ComputeMaxBounds () {
	return this->GetItemBounds ( 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::DrawIndex ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	UNUSED ( idx );
	UNUSED ( xOff );
	UNUSED ( yOff );
	UNUSED ( zOff );
	UNUSED ( xScl );
	UNUSED ( yScl );
	UNUSED ( zScl );

	// TODO: make use of offset and scale
	
	//if ( !this->mVertexBuffer ) return;
	
	//const MOAIVertexFormat* format = this->mVertexBuffer->GetVertexFormat ();
	//if ( !format ) return;

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	gfxDevice.Flush (); // TODO: should remove this call

	this->DoCPUAffirm ();

	if ( this->Bind ()) {

		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_MODEL );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		gfxDevice.SetGfxState ( this->mTexture );
		
		gfxDevice.SetPenWidth ( this->mPenWidth );
		gfxDevice.SetPointSize ( this->mPointSize );
		
		gfxDevice.UpdateShaderGlobals ();
		
		// TODO: use gfxDevice to cache buffers
		if ( this->mIndexBuffer ) {
			//if ( this->mIndexBuffer->Bind ()) {
				this->mIndexBuffer->Draw ( this->mPrimType );
			//	this->mIndexBuffer->Unbind ();
			//}
		}
		else {
			//zglDrawArrays ( this->mPrimType, 0, this->mVertexBuffer->GetTotalElements ());
			zglDrawArrays ( this->mPrimType, 0, this->mTotalElements );
		}
		//this->mVertexBuffer->Unbind ();
	}
}

//----------------------------------------------------------------//
ZLBox MOAIMesh::GetItemBounds ( u32 idx ) {
	UNUSED ( idx );
	
	ZLBox bounds;
	
//	if ( this->mVertexBuffer && this->mVertexBuffer->HasBounds ()) {
//		this->mVertexBuffer->GetBounds ( bounds );
//	}
//	else {
//		bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
//	}

	bounds.Init ( -100.0f, -100.0f, -100.0f, 100.0f, 100.0f, 100.0f );

	return bounds;
}

//----------------------------------------------------------------//
MOAIMesh::MOAIMesh () :
	mPrimType ( ZGL_PRIM_TRIANGLES ),
	mPenWidth ( 1.0f ),
	mPointSize ( 1.0f ),
	mNeedsRefresh ( false ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END
	
	this->mDefaultShaderID = MOAIShaderMgr::MESH_SHADER;
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIMesh::~MOAIMesh () {

	this->ReserveVAOs ( 0 );
	this->ReserveVertexBuffers ( 0 );
	this->SetIndexBuffer ( 0 );
}

//----------------------------------------------------------------//
bool MOAIMesh::OnCPUCreate () {

	return true;
}

//----------------------------------------------------------------//
void MOAIMesh::OnCPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIMesh::OnGPUBind () {

	//if ( !this->mVAOs.Size ()) return;
	
	//if ( this->mNeedsRefresh ) {
	//	this->mCurrentVAO = ( this->mCurrentVAO + 1 ) % this->mVAOs.Size ();
	//}
	
	//zglBindVertexArray ( this->mVAOs [ this->mCurrentVAO ]);
	
	if ( this->mNeedsRefresh ) {
	
		this->mTotalElements = 0;
	
		bool hasBounds = false;
		this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	
		size_t totalVBOs = this->mVertexBuffers.Size ();
		for ( size_t i = 0; i < totalVBOs; ++i ) {
			MOAIVertexBuffer& vertexBuffer = this->mVertexBuffers [ i ];
			
			MOAIGfxBuffer* buffer = vertexBuffer.mBuffer;
			MOAIVertexFormat* format = vertexBuffer.mFormat;
			
			if ( buffer && format ) {
			
				if ( buffer->IsDirty ()) {
					size_t length = buffer->GetLength ();
					vertexBuffer.mTotalElements = ( u32 )( length / format->GetVertexSize ());
					vertexBuffer.mHasBounds = vertexBuffer.mFormat->ComputeBounds ( vertexBuffer.mBounds, buffer, buffer->GetLength ());
				}
				
				if (( this->mTotalElements == 0 ) || ( vertexBuffer.mTotalElements < this->mTotalElements )) {
					this->mTotalElements = vertexBuffer.mTotalElements;
				}
				
				if ( vertexBuffer.mHasBounds ) {
					if ( hasBounds ) {
						this->mBounds.Grow ( vertexBuffer.mBounds );
					}
					else {
						this->mBounds = vertexBuffer.mBounds;
					}
				}
				
				vertexBuffer.mBuffer->Bind ();
				MOAIGfxDevice::Get ().SetVertexFormat ( *vertexBuffer.mFormat, 0 );
			}
		}
		
		this->mNeedsRefresh = false;
	}
	else {
		size_t totalVBOs = this->mVertexBuffers.Size ();
		for ( size_t i = 0; i < totalVBOs; ++i ) {
			MOAIVertexBuffer& vertexBuffer = this->mVertexBuffers [ i ];
			vertexBuffer.mBuffer->Bind ();
			MOAIGfxDevice::Get ().SetVertexFormat ( *vertexBuffer.mFormat, 0 );
		}
	}
	
	if ( this->mIndexBuffer ) {
		this->mIndexBuffer->Bind ();
	}
}

//----------------------------------------------------------------//
bool MOAIMesh::OnGPUCreate () {

//	size_t totalVAOs = this->mVAOs.Size ();
//	for ( size_t i = 0; i < totalVAOs; ++i ) {
//		u32 vao = zglCreateVertexArray ();
//		if ( !vao ) return false;
//		this->mVAOs [ i ] = vao;
//	}
	return true;
}

//----------------------------------------------------------------//
void MOAIMesh::OnGPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIMesh::OnGPULost () {

	this->mVAOs.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::OnGPUUnbind () {
}

//----------------------------------------------------------------//
void MOAIMesh::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
	MOAIGfxResource::RegisterLuaClass ( state );
	
	state.SetField ( -1, "GL_POINTS",			( u32 )ZGL_PRIM_POINTS );
	state.SetField ( -1, "GL_LINES",			( u32 )ZGL_PRIM_LINES );
	state.SetField ( -1, "GL_TRIANGLES",		( u32 )ZGL_PRIM_TRIANGLES );
	state.SetField ( -1, "GL_LINE_LOOP",		( u32 )ZGL_PRIM_LINE_LOOP );
	state.SetField ( -1, "GL_LINE_STRIP",		( u32 )ZGL_PRIM_LINE_STRIP );
	state.SetField ( -1, "GL_TRIANGLE_FAN",		( u32 )ZGL_PRIM_TRIANGLE_FAN );
	state.SetField ( -1, "GL_TRIANGLE_STRIP",	( u32 )ZGL_PRIM_TRIANGLE_STRIP );
}

//----------------------------------------------------------------//
void MOAIMesh::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );
	MOAIGfxResource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "reserveVAOs",			_reserveVAOs },
		{ "reserveVertexBuffers",	_reserveVertexBuffers },
		{ "setIndexBuffer",			_setIndexBuffer },
		{ "setPenWidth",			_setPenWidth },
		{ "setPointSize",			_setPointSize },
		{ "setPrimType",			_setPrimType },
		{ "setVertexBuffer",		_setVertexBuffer },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMesh::ReserveVAOs ( u32 total ) {

	for ( size_t i = 0; i < this->mVAOs.Size (); ++i ) {
		MOAIGfxResourceMgr::Get ().PushDeleter ( MOAIGfxDeleter::DELETE_BUFFER, this->mVAOs [ i ]);
	}
	this->mVAOs.Init ( total );
	this->mVAOs.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::ReserveVertexBuffers ( u32 total ) {

	for ( size_t i = 0; i < this->mVertexBuffers.Size (); ++i ) {
		this->mVertexBuffers [ i ].SetBufferAndFormat ( *this, 0, 0 );
	}
	this->mVertexBuffers.Init ( total );
}

//----------------------------------------------------------------//
void MOAIMesh::SetIndexBuffer ( MOAIIndexBuffer* indexBuffer ) {

	this->mIndexBuffer.Set ( *this, indexBuffer );
	this->mNeedsRefresh;
}

//----------------------------------------------------------------//
void MOAIMesh::SetVertexBuffer ( u32 idx, MOAIGfxBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) {

	if (( this->mVertexBuffers.Size () == 0 ) && ( idx == 0 )) {
		this->ReserveVertexBuffers ( 1 );
	}
	
	if ( idx < this->mVertexBuffers.Size ()) {
		this->mVertexBuffers [ idx ].SetBufferAndFormat ( *this, vtxBuffer, vtxFormat );
		this->mNeedsRefresh = true;
	}
}
