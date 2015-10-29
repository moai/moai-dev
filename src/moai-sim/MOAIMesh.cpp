// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

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

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->ClearBounds ();
	
	if ( state.CheckParams ( 2, "NNNNNN-" )) {
		self->SetBounds ( state.GetValue < ZLBox >( 2, self->mBounds ));
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
/**	@lua	setPrimType
	@text	Sets the prim type the buffer represents.
	
	@in		MOAIMesh self
	@in		number primType		One of MOAIMesh GL_POINTS, GL_LINES, GL_TRIANGLES, GL_LINE_LOOP,
								GL_LINE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP
	@out	nil
*/
int MOAIMesh::_setPrimType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "UN" )
	
	self->SetPrimType ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------///
// TODO: doxygen
int MOAIMesh::_setTotalElements ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	self->SetTotalElements ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//================================================================//
// MOAIMesh
//================================================================//

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
void MOAIMesh::DrawIndex ( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {

	this->DrawIndex ( idx, 0, materials, offset, scale );
}

//----------------------------------------------------------------//
void MOAIMesh::DrawIndex ( u32 idx, MOAIMeshSpan* span, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {
	UNUSED ( offset );
	UNUSED ( scale );

	materials.LoadGfxState ( this, idx, MOAIShaderMgr::MESH_SHADER );

	// TODO: make use of offset and scale

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	this->FinishInit ();
	gfxDevice.BindVertexArray ( this );

	if ( this->IsReady ()) {

		// I am super lazy, so set this up here instead of adding if's below
		MOAIMeshSpan defaultSpan;
		if ( !span ) {
			defaultSpan.mBase = 0;
			defaultSpan.mTop = this->mTotalElements;
			defaultSpan.mNext = 0;
			span = &defaultSpan;
		}

		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_MODEL );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		
		gfxDevice.SetPenWidth ( this->mPenWidth );
		
		gfxDevice.UpdateShaderGlobals ();
		
		// TODO: use gfxDevice to cache buffers
		if ( this->mIndexBuffer ) {
			gfxDevice.BindIndexBuffer ( this->mIndexBuffer );
			
			if ( this->mIndexBuffer->IsReady ()) {
			
				u32 indexSizeInBytes = this->mIndexBuffer->GetIndexSize ();
				
				for ( ; span; span = span->mNext ) {
					zglDrawElements (
						this->mPrimType,
						span->mTop - span->mBase,
						indexSizeInBytes == 2 ? ZGL_TYPE_UNSIGNED_SHORT : ZGL_TYPE_UNSIGNED_INT,
						( const void* )(( size_t )this->mIndexBuffer->GetAddress () + ( span->mBase * indexSizeInBytes ))
					);
				}
				gfxDevice.BindIndexBuffer ();
			}
		}
		else {
			for ( ; span; span = span->mNext ) {
				zglDrawArrays ( this->mPrimType, span->mBase, span->mTop - span->mBase );
			}
		}
		gfxDevice.BindVertexArray ();
	}
}

//----------------------------------------------------------------//
ZLBox MOAIMesh::GetItemBounds ( u32 idx ) {
	UNUSED ( idx );

	return this->mBounds;
}

//----------------------------------------------------------------//
MOAIMesh::MOAIMesh () :
	mTotalElements ( 0 ),
	mPrimType ( ZGL_PRIM_TRIANGLES ),
	mPenWidth ( 1.0f ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStandardDeck )
		RTTI_EXTEND ( MOAIVertexArray )
	RTTI_END
	
	this->ClearBounds ();
}

//----------------------------------------------------------------//
MOAIMesh::~MOAIMesh () {

	this->SetIndexBuffer ( 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStandardDeck::RegisterLuaClass ( state );
	MOAIVertexArray::RegisterLuaClass ( state );
	
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

	MOAIStandardDeck::RegisterLuaFuncs ( state );
	MOAIVertexArray::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "reserveVAOs",				_reserveVAOs },
		{ "reserveVertexBuffers",		_reserveVertexBuffers },
		{ "setBounds",					_setBounds },
		{ "setIndexBuffer",				_setIndexBuffer },
		{ "setPenWidth",				_setPenWidth },
		{ "setPrimType",				_setPrimType },
		{ "setTotalElements",			_setTotalElements },
		{ "setVertexBuffer",			_setVertexBuffer },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIMesh::ReserveVAOs ( u32 total ) {

	if ( MOAIGfxResourceMgr::IsValid ()) {
		for ( size_t i = 0; i < this->mVAOs.Size (); ++i ) {
			MOAIGfxResourceMgr::Get ().PushDeleter ( MOAIGfxDeleter::DELETE_BUFFER, this->mVAOs [ i ]);
		}
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

	MOAIVertexArray::SerializeIn ( state, serializer );

	this->SetIndexBuffer ( serializer.MemberIDToObject < MOAIIndexBuffer >( state.GetField < MOAISerializer::ObjID >( -1, "mIndexBuffer", 0 )));
	
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
	
	this->mPenWidth = state.GetField < float >( -1, "mPenWidth", 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIVertexArray::SerializeOut ( state, serializer );

	state.SetField ( -1, "mIndexBuffer", serializer.AffirmMemberID ( this->mIndexBuffer ));
	
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
}

//----------------------------------------------------------------//
void MOAIMesh::SetBounds ( const ZLBox& bounds ) {

	this->mBounds = bounds;
	this->mHasBounds = true;
}

//----------------------------------------------------------------//
void MOAIMesh::SetIndexBuffer ( MOAIIndexBuffer* indexBuffer ) {

	this->mIndexBuffer.Set ( *this, indexBuffer );
}
