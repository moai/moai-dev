// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIIndexBuffer.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIMesh.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAIVertexBuffer.h>
#include <moaicore/MOAIVertexFormat.h>

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
	
	self->mIndexBuffer.Set ( *self, state.GetLuaObject < MOAIIndexBuffer >( 2 ));

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
	
	self->mVertexBuffer.Set ( *self, state.GetLuaObject < MOAIVertexBuffer >( 2 ));

	return 0;
}

//================================================================//
// MOAIMesh
//================================================================//

//----------------------------------------------------------------//
void MOAIMesh::Draw ( const USAffine3D& transform, u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );

	if ( !this->mVertexBuffer ) return;
	if ( !this->mVertexBuffer->IsValid ()) return;

	if ( this->mVertexBuffer->Bind ()) {
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();	

		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_MODEL );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		gfxDevice.SetGfxState ( this->mTexture );
		gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, transform );
		
		//gfxDevice.SetPenWidth ( this->mPenWidth );
		//gfxDevice.SetPointSize ( this->mPointSize );
		
		if ( this->mIndexBuffer ) {
			if ( this->mIndexBuffer->LoadGfxState ()) {
				glDrawElements ( this->mPrimType, this->mIndexBuffer->GetIndexCount (), GL_UNSIGNED_SHORT, 0 );
			}
		}
		else {
			glDrawArrays ( this->mPrimType, 0, this->mVertexBuffer->GetVertexCount ());
		}
	}
}

//----------------------------------------------------------------//
void MOAIMesh::Draw ( const USAffine3D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );
}

//----------------------------------------------------------------//
USBox MOAIMesh::GetBounds () {
	return GetBounds ( 0, NULL );
}

//----------------------------------------------------------------//
USBox MOAIMesh::GetBounds ( u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );
	
	if ( this->mVertexBuffer ) {
		return this->mVertexBuffer->GetBounds ();
	}
	USBox bounds;
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	return bounds;
}

//----------------------------------------------------------------//
MOAIGfxState* MOAIMesh::GetShaderDefault () {

	return &MOAIShaderMgr::Get ().GetShader ( MOAIShaderMgr::MESH_SHADER );
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
	
	state.SetField ( -1, "GL_POINTS", ( u32 )GL_POINTS );
	state.SetField ( -1, "GL_LINES", ( u32 )GL_LINES );
	state.SetField ( -1, "GL_TRIANGLES", ( u32 )GL_TRIANGLES );
	state.SetField ( -1, "GL_LINE_LOOP", ( u32 )GL_LINE_LOOP );
	state.SetField ( -1, "GL_LINE_STRIP", ( u32 )GL_LINE_STRIP );
	state.SetField ( -1, "GL_TRIANGLE_FAN", ( u32 )GL_TRIANGLE_FAN );
	state.SetField ( -1, "GL_TRIANGLE_STRIP", ( u32 )GL_TRIANGLE_STRIP );
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
