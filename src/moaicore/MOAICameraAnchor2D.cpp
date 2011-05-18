// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICameraAnchor2D.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAICameraAnchor2D::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraAnchor2D, "U" )
	
	MOAITransformBase* parent = state.GetLuaObject < MOAITransformBase >( 2 );
	self->SetParent ( parent );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAICameraAnchor2D::_setScreenRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraAnchor2D, "UNNNN" )

	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float x1	= state.GetValue < float >( 4, 0.0f );
	float y1	= state.GetValue < float >( 5, 0.0f );
	
	self->mScreenRect.Init ( x0, y0, x1, y1 );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAICameraAnchor2D::_setWorldRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraAnchor2D, "UNNNN" )

	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float x1	= state.GetValue < float >( 4, 0.0f );
	float y1	= state.GetValue < float >( 5, 0.0f );
	
	self->mWorldRect.Init ( x0, y0, x1, y1 );
	
	return 0;
}

//================================================================//
// MOAICameraAnchor2D
//================================================================//

//----------------------------------------------------------------//
USRect MOAICameraAnchor2D::GetScreenRect ( const USAffine2D& worldToWnd ) {

	USVec2D screenLoc = this->mWorldLoc;
	worldToWnd.Transform ( screenLoc );
	
	USRect rect = this->mScreenRect;
	rect.Offset ( screenLoc.mX, screenLoc.mY );
	rect.Bless ();
	
	return rect;
}

//----------------------------------------------------------------//
USRect MOAICameraAnchor2D::GetWorldRect () {

	USRect rect = this->mWorldRect;
	rect.Offset ( this->mWorldLoc.mX, this->mWorldLoc.mY );
	rect.Bless ();
	
	return rect;
}

//----------------------------------------------------------------//
MOAICameraAnchor2D::MOAICameraAnchor2D () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
	RTTI_END
	
	this->mScreenRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mWorldRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mWorldLoc.Init ( 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAICameraAnchor2D::~MOAICameraAnchor2D () {
}

//----------------------------------------------------------------//
void MOAICameraAnchor2D::OnDepNodeUpdate () {
	
	if ( this->mParent ) {
	
		const USAffine2D& worldMtx = this->mParent->GetLocalToWorldMtx ();
		this->mWorldLoc = worldMtx.GetTranslation ();
	}
}

//----------------------------------------------------------------//
void MOAICameraAnchor2D::RegisterLuaClass ( USLuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAICameraAnchor2D::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setParent",			_setParent },
		{ "setScreenRect",		_setScreenRect },
		{ "setWorldRect",		_setWorldRect },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICameraAnchor2D::SetParent ( MOAITransformBase* parent ) {

	if ( this->mParent == parent ) return;
	
	if ( this->mParent ) {
		this->ClearDependency ( *this->mParent );
	}

	this->mParent = parent;
	
	if ( parent ) {
		this->SetDependency ( *parent );
	}

	this->ScheduleUpdate ();
}

//----------------------------------------------------------------//
STLString MOAICameraAnchor2D::ToString () {

	STLString repr( MOAINode::ToString ());
	return repr;
}