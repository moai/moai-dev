// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIMesh.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAIVertexBuffer.h>
#include <moaicore/MOAIVertexFormat.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_setTexture ( lua_State* L ) {
	LUA_SETUP ( MOAIMesh, "U" )

	self->mTexture = MOAITexture::AffirmTexture ( state, 2 );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMesh::_setVertexBuffer ( lua_State* L ) {
	LUA_SETUP ( MOAIMesh, "U" )
	
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
void MOAIMesh::Draw ( const USAffine2D& transform, u32 idx ) {
	UNUSED ( idx );
	
	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	drawBuffer.Flush ();
	
	USCanvas::SetWorldMtx ( transform );
	this->mVertexBuffer->Draw ();
	
	drawBuffer.Reset ();
}

//----------------------------------------------------------------//
void MOAIMesh::Draw ( const USAffine2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( c0 );
	UNUSED ( c1 );
}

//----------------------------------------------------------------//
USRect MOAIMesh::GetBounds ( u32 idx ) {
	UNUSED ( idx );
	
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

	LuaReg regTable [] = {
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
