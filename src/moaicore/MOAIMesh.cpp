// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIMesh.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAIVertexBuffer.h>
#include <moaicore/MOAIVertexFormat.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setTexture
	@text	Set or load a texture for this deck.
	
	@in		MOAIMesh self
	@in		variant texture		A MOAITexture, a MOAIDataBuffer or a path to a texture file
	@opt	number transform	Any bitwise combination of MOAITexture.QUANTIZE, MOAITexture.TRUECOLOR, MOAITexture.PREMULTIPLY_ALPHA
	@out	MOAITexture texture
*/
int MOAIMesh::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMesh, "U" )

	self->mTexture = MOAITexture::AffirmTexture ( state, 2 );
	if ( self->mTexture ) {
		self->mTexture->PushLuaUserdata ( state );
		return 1;
	}
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
	
	self->mVertexBuffer = state.GetLuaObject < MOAIVertexBuffer >( 2 );

	return 0;
}

//================================================================//
// MOAIMesh
//================================================================//

//----------------------------------------------------------------//
bool MOAIMesh::Bind () {

	if ( !this->mVertexBuffer ) return false;
	if ( !this->mVertexBuffer->IsValid ()) return false;

	return true;
}

//----------------------------------------------------------------//
void MOAIMesh::Draw ( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );
	
	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	
	drawBuffer.SetTexture ( this->mTexture );
	drawBuffer.SetVtxTransform ( transform );

	this->mVertexBuffer->Draw ();
}

//----------------------------------------------------------------//
void MOAIMesh::Draw ( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );
}

//----------------------------------------------------------------//
USRect MOAIMesh::GetBounds ( u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );
	
	if ( this->mVertexBuffer ) {
		return this->mVertexBuffer->GetBounds ();
	}
	USRect bounds;
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	return bounds;
}

//----------------------------------------------------------------//
MOAIMesh::MOAIMesh () {

	RTTI_SINGLE ( MOAIDeck )
	this->SetContentMask ( MOAIProp::CAN_DRAW );
}

//----------------------------------------------------------------//
MOAIMesh::~MOAIMesh () {
}

//----------------------------------------------------------------//
void MOAIMesh::RegisterLuaClass ( USLuaState& state ) {

	this->MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIMesh::RegisterLuaFuncs ( USLuaState& state ) {

	this->MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setTexture",				_setTexture },
		{ "setVertexBuffer",		_setVertexBuffer },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIMesh::ToString () {

	STLString repr;

	return repr;
}
