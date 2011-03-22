// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICameraFitter.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>() addPin ( self, x, y, radius )</tt>\n
\n
	Sets a fitting pin. X and Y is in world space; radius in screen space.
	@param self (in)
	@param x (in)
	@param y (in)
	@param radius (in)
*/
int MOAICameraFitter::_addPin ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UNNN" )) return 0;
	
	MOAICameraFitter* self = state.GetLuaData < MOAICameraFitter >( 1 );
	if ( !self ) return 0;

	float x = state.GetValue < float >( 2, 0 );
	float y = state.GetValue < float >( 3, 0 );
	float radius = state.GetValue < float >( 4, 0 );

	self->AddPin ( x, y, radius );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) center ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICameraFitter::_center ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAICameraFitter* self = state.GetLuaData < MOAICameraFitter >( 1 );
	if ( !self ) return 0;
	
	self->Center ();

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICameraFitter::_fit ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAICameraFitter* self = state.GetLuaData < MOAICameraFitter >( 1 );
	if ( !self ) return 0;

	self->Fit ();

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICameraFitter::_reset ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAICameraFitter* self = state.GetLuaData < MOAICameraFitter >( 1 );
	if ( !self ) return 0;

	self->Reset ();

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICameraFitter::_setCamera ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UU" )) return 0;
	
	MOAICameraFitter* self = state.GetLuaData < MOAICameraFitter >( 1 );
	if ( !self ) return 0;
	
	USCamera2D* camera = state.GetLuaData < USCamera2D >( 2 );
	if ( camera ) {
		self->SetCamera ( camera );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICameraFitter::_setViewport ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UU" )) return 0;
	
	MOAICameraFitter* self = state.GetLuaData < MOAICameraFitter >( 1 );
	if ( !self ) return 0;
	
	USViewport* viewport = state.GetLuaData < USViewport >( 2 );
	if ( viewport ) {
		self->SetViewport ( viewport );
	}

	return 0;
}

//================================================================//
// MOAICameraFitter
//================================================================//

//----------------------------------------------------------------//
void MOAICameraFitter::AddPin ( float x, float y, float radius ) {

	USVec2D loc ( x, y );
	this->mFitter.AddPin ( loc, radius );
}

//----------------------------------------------------------------//
void MOAICameraFitter::Center () {

	if ( this->mCamera && this->mViewport ) {
		this->mFitter.Center ( *this->mCamera, *this->mViewport );
		this->mCamera->mLoc = this->mFitter.mTargetLoc;
	}
}

//----------------------------------------------------------------//
void MOAICameraFitter::Fit () {

	if ( this->mCamera && this->mViewport ) {
		USRect fittingRect = this->mViewport->GetRect ();
		this->mFitter.Fit ( *this->mCamera, *this->mViewport, fittingRect, 8 );
		this->mCamera->mLoc = this->mFitter.mTargetLoc;
		this->mCamera->mScale = this->mFitter.mTargetScale;
	}
}

//----------------------------------------------------------------//
MOAICameraFitter::MOAICameraFitter () {
	
	RTTI_SINGLE ( USLuaData )
}

//----------------------------------------------------------------//
MOAICameraFitter::~MOAICameraFitter () {
}

//----------------------------------------------------------------//
void MOAICameraFitter::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAICameraFitter::RegisterLuaFuncs ( USLuaState& state ) {
	
	LuaReg regTable [] = {
		{ "addPin",			_addPin },
		{ "center",			_center },
		{ "fit",			_fit },
		{ "reset",			_reset },
		{ "setCamera",		_setCamera },
		{ "setViewport",	_setViewport },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICameraFitter::Reset () {

	this->mFitter.Reset ();
}

//----------------------------------------------------------------//
STLString MOAICameraFitter::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mCamera )
	PRETTY_PRINT ( repr, mViewport )

	// TODO Info about pins is not accessible

	return repr;
}
