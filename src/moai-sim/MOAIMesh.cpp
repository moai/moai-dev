// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxBuffer.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// MOAIVertexBuffer
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexBuffer::Bind () {

	if ( this->mBuffer && this->mFormat ) {
		this->mBuffer->Bind ();
		this->mFormat->Bind ( 0 );
	}
}

//----------------------------------------------------------------//
MOAIVertexBuffer::MOAIVertexBuffer () {
}

//----------------------------------------------------------------//
MOAIVertexBuffer::~MOAIVertexBuffer () {
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::SetBufferAndFormat ( MOAIMesh& owner, MOAIGfxBuffer* buffer, MOAIVertexFormat* format ) {

	this->mBuffer.Set ( owner, buffer );
	this->mFormat.Set ( owner, format );
}

//----------------------------------------------------------------//
void MOAIVertexBuffer::Unbind () {

	if ( this->mBuffer && this->mFormat ) {
		this->mBuffer->Unbind ();
		this->mFormat->Unbind ();
	}
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
// TODO: doxygen
int MOAIMesh::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->ClearBounds ();
	
	if ( state.CheckParams ( 2, "NNNNNN-" )) {
		self->mHasBounds = true;
		self->mBounds = state.GetValue < ZLBox >( 2, self->mBounds );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setIndexBuffer
	@text	Set the index buffer to render.
	
	@in		MOAIMesh self
	@in		MOAIGfxBuffer indexBuffer
	@out	nil
*/
int MOAIMesh::_setIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->SetIndexBuffer ( state.GetLuaObject < MOAIGfxBuffer >( 2, true ));
	self->mIndexSizeInBytes = state.GetValue < u32 >( 3, 2 );
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

//----------------------------------------------------------------///
// TODO: doxygen
int MOAIMesh::_setTotalElements ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	self->mTotalElements = state.GetValue < u32 >( 2, 0 );
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
	self->FinishInit ();

	return 0;
}

//================================================================//
// MOAIMesh
//================================================================//

//----------------------------------------------------------------//
bool MOAIMesh::AffirmVertexBuffers ( u32 idx ) {

	if (( this->mVertexBuffers.Size () == 0 ) && ( idx == 0 )) {
		this->ReserveVertexBuffers ( 1 );
	}
	return ( idx < this->mVertexBuffers.Size ());
}

//----------------------------------------------------------------//
void MOAIMesh::BindVertex () {

	size_t totalVBOs = this->mVertexBuffers.Size ();
	for ( size_t i = 0; i < totalVBOs; ++i ) {
		this->mVertexBuffers [ i ].Bind ();
	}

	if ( this->mIndexBuffer ) {
		this->mIndexBuffer->Bind ();
	}
}

//----------------------------------------------------------------//
void MOAIMesh::ClearBounds () {

	this->mHasBounds = false;
	this->mBounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
}

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

	this->FinishInit ();

	if ( this->Bind ()) {

		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_MODEL );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		gfxDevice.SetGfxState ( this->mTexture );
		
		gfxDevice.SetPenWidth ( this->mPenWidth );
		gfxDevice.SetPointSize ( this->mPointSize );
		
		gfxDevice.UpdateShaderGlobals ();
		
		// TODO: use gfxDevice to cache buffers
		if ( this->mIndexBuffer ) {
			if ( this->mIndexBuffer->Bind ()) {
				zglDrawElements ( this->mPrimType, this->mTotalElements, this->mIndexSizeInBytes == 2 ? ZGL_TYPE_UNSIGNED_SHORT : ZGL_TYPE_UNSIGNED_INT, 0 );
			}
		}
		else {
			zglDrawArrays ( this->mPrimType, 0, this->mTotalElements );
		}
		this->Unbind ();
	}
}

//----------------------------------------------------------------//
ZLBox MOAIMesh::GetItemBounds ( u32 idx ) {
	UNUSED ( idx );

	return this->mBounds;
}

//----------------------------------------------------------------//
MOAIMesh::MOAIMesh () :
	mIndexSizeInBytes ( 0 ),
	mTotalElements ( 0 ),
	mPrimType ( ZGL_PRIM_TRIANGLES ),
	mPenWidth ( 1.0f ),
	mPointSize ( 1.0f ),
	mUseVAOs ( false ),
	mNeedsRefresh ( false ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END
	
	this->mDefaultShaderID = MOAIShaderMgr::MESH_SHADER;
	
	this->ClearBounds ();
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

	u32 vao = 0;

	if ( this->mUseVAOs && this->mNeedsRefresh ) {
	
		if ( !this->mVAOs.Size ()) return;
	
		if ( this->mNeedsRefresh ) {
			this->mCurrentVAO = ( this->mCurrentVAO + 1 ) % this->mVAOs.Size ();
		}
		vao = this->mVAOs [ this->mCurrentVAO ];
	}
	
	if ( vao ) {

		zglBindVertexArray ( vao );

		if ( this->mNeedsRefresh ) {
			this->BindVertex ();
			this->mNeedsRefresh = false;
		}
	}
	else {
	
		this->BindVertex ();
	}
}

//----------------------------------------------------------------//
bool MOAIMesh::OnGPUCreate () {

	size_t totalVAOs = this->mVAOs.Size ();
	
	if ( !totalVAOs ) {
		totalVAOs = 1;
		this->ReserveVAOs ( totalVAOs );
	}
	
	this->mUseVAOs = false;
	
	for ( size_t i = 0; i < totalVAOs; ++i ) {
		u32 vao = zglCreateVertexArray (); // OK for this to return 0
		if ( !vao ) return true;
		this->mVAOs [ i ] = vao;
	}
	
	this->mUseVAOs = true;
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

	if ( this->mUseVAOs ) {
		zglBindVertexArray ( 0 );
	}
	this->UnbindVertex ();
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
		{ "reserveVAOs",				_reserveVAOs },
		{ "reserveVertexBuffers",		_reserveVertexBuffers },
		{ "setBounds",					_setBounds },
		{ "setIndexBuffer",				_setIndexBuffer },
		{ "setPenWidth",				_setPenWidth },
		{ "setPointSize",				_setPointSize },
		{ "setPrimType",				_setPrimType },
		{ "setTotalElements",			_setTotalElements },
		{ "setVertexBuffer",			_setVertexBuffer },
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
void MOAIMesh::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	u32 totalVAOs = state.GetField < u32 >( -1, "mTotalVAOs", 0 );
	this->ReserveVAOs ( totalVAOs );
	
	u32 totalVertexBuffers = state.GetField < u32 >( -1, "mTotalVertexBuffers", 0 );
	this->ReserveVertexBuffers ( totalVertexBuffers );
	
	if ( state.GetFieldWithType ( -1, "mVertexBuffers", LUA_TTABLE )) {
		int itr = state.PushTableItr ( -1 );
		for ( u32 i = 0; state.TableItrNext ( itr ); ++i ) {
			if ( state.IsType ( -1, LUA_TTABLE )) {
				MOAIGfxBuffer* buffer = serializer.MemberIDToObject < MOAIGfxBuffer >( state.GetField < MOAISerializer::ObjID >( -1, "mBuffer", 0 ));
				MOAIVertexFormat* format = serializer.MemberIDToObject < MOAIVertexFormat >( state.GetField < MOAISerializer::ObjID >( -1, "mFormat", 0 ));
				this->SetVertexBuffer ( i, buffer, format );
			}
		}
		state.Pop ();
	}
	
	this->SetIndexBuffer ( serializer.MemberIDToObject < MOAIGfxBuffer >( state.GetField < MOAISerializer::ObjID >( -1, "mIndexBuffer", 0 )));
	
	this->mIndexSizeInBytes = state.GetField < u32 >( -1, "mIndexSizeInBytes", 0 );
	this->mTotalElements = state.GetField < u32 >( -1, "mTotalElements", 0 );
	
	this->mHasBounds = state.GetField < bool >( -1, "mHasBounds", 0 );
	
	if ( state.GetFieldWithType ( -1, "mBounds", LUA_TTABLE )) {
		
		this->mBounds.mMin.mX	= state.GetField < float >( -1, "mMinX", 0 );
		this->mBounds.mMin.mY	= state.GetField < float >( -1, "mMinY", 0 );
		this->mBounds.mMin.mZ	= state.GetField < float >( -1, "mMinZ", 0 );
		
		this->mBounds.mMax.mX	= state.GetField < float >( -1, "mMaxX", 0 );
		this->mBounds.mMax.mY	= state.GetField < float >( -1, "mMaxY", 0 );
		this->mBounds.mMax.mZ	= state.GetField < float >( -1, "mMaxZ", 0 );
		
		state.Pop ();
	}
	
	this->mPenWidth		= state.GetField < float >( -1, "mPenWidth", 0 );
	this->mPointSize	= state.GetField < float >( -1, "mPointSize", 0 );
	
	this->mNeedsRefresh = true;
	
	this->FinishInit ();
}

//----------------------------------------------------------------//
void MOAIMesh::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	state.SetField < u32 >( -1, "mTotalVAOs", this->mVAOs.Size ());
	state.SetField < u32 >( -1, "mTotalVertexBuffers", this->mVertexBuffers.Size ());
	
	lua_newtable ( state );
	for ( u32 i = 0; i < this->mVertexBuffers.Size (); ++i ) {
		state.Push ( i + 1 );
		lua_newtable ( state );
		state.SetField < MOAISerializer::ObjID >( -1, "mBuffer", serializer.AffirmMemberID ( this->mVertexBuffers [ i ].mBuffer ));
		state.SetField < MOAISerializer::ObjID >( -1, "mFormat", serializer.AffirmMemberID ( this->mVertexBuffers [ i ].mFormat ));
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mVertexBuffers" );
	
	state.SetField ( -1, "mIndexBuffer", serializer.AffirmMemberID ( this->mIndexBuffer ));
	
	state.SetField < u32 >( -1, "mIndexSizeInBytes", this->mIndexSizeInBytes );
	state.SetField < u32 >( -1, "mTotalElements", this->mTotalElements );
	
	state.SetField < bool >( -1, "mHasBounds", this->mHasBounds );
	
	lua_newtable ( state );
	
		state.SetField < float >( -1, "mMinX", this->mBounds.mMin.mX );
		state.SetField < float >( -1, "mMinY", this->mBounds.mMin.mY );
		state.SetField < float >( -1, "mMinZ", this->mBounds.mMin.mZ );
	
		state.SetField < float >( -1, "mMaxX", this->mBounds.mMax.mX );
		state.SetField < float >( -1, "mMaxY", this->mBounds.mMax.mY );
		state.SetField < float >( -1, "mMaxZ", this->mBounds.mMax.mZ );
	
	lua_setfield ( state, -2, "mBounds" );
	
	state.SetField < u32 >( -1, "mPenWidth", this->mPenWidth );
	state.SetField < u32 >( -1, "mPointSize", this->mPointSize );
}

//----------------------------------------------------------------//
void MOAIMesh::SetIndexBuffer ( MOAIGfxBuffer* indexBuffer ) {

	this->mIndexBuffer.Set ( *this, indexBuffer );
	this->mNeedsRefresh = true;
}

//----------------------------------------------------------------//
void MOAIMesh::SetVertexBuffer ( u32 idx, MOAIGfxBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) {

	if ( this->AffirmVertexBuffers ( idx )) {
		this->mVertexBuffers [ idx ].SetBufferAndFormat ( *this, vtxBuffer, vtxFormat );
		this->mNeedsRefresh = true;
	}
}

//----------------------------------------------------------------//
void MOAIMesh::UnbindVertex () {

	size_t totalVBOs = this->mVertexBuffers.Size ();
	for ( size_t i = 0; i < totalVBOs; ++i ) {
		this->mVertexBuffers [ i ].Unbind ();
	}

	if ( this->mIndexBuffer ) {
		this->mIndexBuffer->Unbind ();
	}
}
