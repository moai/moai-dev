// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setIndexBuffer
	@text	Set the index buffer to render.
	
	@in		MOAIMesh self
	@in		MOAIIndexBuffer indexBuffer
	@out	nil
*/
int MOAIMesh::_setIndexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->mIndexBuffer.Set ( *self, state.GetLuaObject < MOAIIndexBuffer >( 2, true ));

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPenWidth
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
/**	@name	setPointSize
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
/**	@name	setPrimType
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
/**	@name	setVertexBuffer
	@text	Set the vertex buffer to render.
	
	@in		MOAIMesh self
	@in		MOAIVertexBuffer vertexBuffer
	@out	nil
*/
int MOAIMesh::_setVertexBuffer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )
	
	self->mVertexBuffer.Set ( *self, state.GetLuaObject < MOAIVertexBuffer >( 2, true ));
	self->SetBoundsDirty ();

	return 0;
}

//================================================================//
// MOAIMesh
//================================================================//

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

	if ( !this->mVertexBuffer ) return;
	if ( !this->mVertexBuffer->IsValid ()) return;

	if ( this->mVertexBuffer->Bind ()) {
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();	

		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_MODEL );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		gfxDevice.SetGfxState ( this->mTexture );
		
		gfxDevice.SetPenWidth ( this->mPenWidth );
		gfxDevice.SetPointSize ( this->mPointSize );
		
		// TODO: use gfxDevice to cache buffers
		if ( this->mIndexBuffer ) {
			if ( this->mIndexBuffer->LoadGfxState ()) {
				zglDrawElements ( this->mPrimType, this->mIndexBuffer->GetIndexCount (), ZGL_TYPE_UNSIGNED_SHORT, 0 );
			}
		}
		else {
			zglDrawArrays ( this->mPrimType, 0, this->mVertexBuffer->GetVertexCount ());
		}
	}
}

//----------------------------------------------------------------//
ZLBox MOAIMesh::GetItemBounds ( u32 idx ) {
	UNUSED ( idx );
	
	if ( this->mVertexBuffer ) {
		return this->mVertexBuffer->GetBounds ();
	}
	
	ZLBox bounds;
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	return bounds;
}

//----------------------------------------------------------------//
MOAIMesh::MOAIMesh () :
	mPrimType ( 0 ),
	mPenWidth ( 1.0f ),
	mPointSize ( 1.0f ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
	
	this->SetContentMask ( MOAIProp::CAN_DRAW );
	this->mDefaultShaderID = MOAIShaderMgr::MESH_SHADER;
}

//----------------------------------------------------------------//
MOAIMesh::~MOAIMesh () {

	this->mIndexBuffer.Set ( *this, 0 );
	this->mTexture.Set ( *this, 0 );
	this->mVertexBuffer.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIMesh::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
	
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

	luaL_Reg regTable [] = {
		{ "setIndexBuffer",			_setIndexBuffer },
		{ "setPenWidth",			_setPenWidth },
		{ "setPointSize",			_setPointSize },
		{ "setPrimType",			_setPrimType },
		{ "setVertexBuffer",		_setVertexBuffer },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
