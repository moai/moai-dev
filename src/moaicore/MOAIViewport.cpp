// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIViewport.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setOffset
	@text	Sets the viewport offset in normalized view space (size of
			viewport is -1 to 1 in both directions).
	
	@in		MOAIViewport self
	@in		number xOff
	@in		number yOff
	@out	nil
*/
int MOAIViewport::_setOffset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIViewport, "UNN" )
	
	float xOffset = state.GetValue < float >( 2, 0.0f );
	float yOffset = state.GetValue < float >( 3, 0.0f );

	self->SetOffset ( xOffset, yOffset );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRotation
	@text	Sets global rotation to be added to camera transform.
	
	@in		MOAIViewport self
	@in		number rotation
	@out	nil
*/
int MOAIViewport::_setRotation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIViewport, "U" )
	
	float rotation = state.GetValue < float >( 2, 0.0f );
	self->SetRotation ( rotation );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setScale
	@text	Sets the number of world units visible of the viewport for one or both
			dimensions. Set 0 for one of the dimensions to use a derived value based on
			the other dimension and the aspect ratio. Negative values are also OK.
			
			It is typical to set the scale to the number of pixels visible in the
			this-> This practice is neither endorsed nor condemned.
			
			Note that the while the contents of the viewport will appear to stretch
			or shrink to match the dimensions of the viewport given by setSize, the
			number of world units visible will remain constant.
	
	@in		MOAIViewport self
	@in		number xScale
	@in		number yScale
	@out	nil
*/
int MOAIViewport::_setScale ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UNN" )) return 0;
	
	MOAIViewport* self = state.GetLuaObject < MOAIViewport >( 1, true );
	if ( !self ) return 0;

	float xScale = state.GetValue < float >( 2, 0.0f );
	float yScale = state.GetValue < float >( 3, 0.0f );
	
	self->SetScale ( xScale, yScale );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSize
	@text	Sets the dimensions of the this->
	
	
	@overload
	
		@in		MOAIViewport self
		@in		number width
		@in		number height
		@out	nil
		
	@overload
	
		@in		MOAIViewport self
		@in		number left
		@in		number top
		@in		number right
		@in		number bottom
		@out	nil
*/
int MOAIViewport::_setSize ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UNN" )) return 0;
	
	MOAIViewport* self = state.GetLuaObject < MOAIViewport >( 1, true );
	if ( !self ) return 0;

	float x0 = state.GetValue < float >( 2, 0.0f );
	float y0 = state.GetValue < float >( 3, 0.0f );

	if ( state.CheckParams ( 4, "NN", false )) {
	
		float x1 = state.GetValue < float >( 4, 0.0f );
		float y1 = state.GetValue < float >( 5, 0.0f );
		
		self->Init ( x0, y0, x1, y1 );
	}
	else {
		self->Init ( 0.0f, 0.0f, x0, y0 );
	}

	return 0;
}

//================================================================//
// MOAIViewport
//================================================================//

//----------------------------------------------------------------//
float MOAIViewport::GetAspect () const {
	return this->Width () / this->Height ();
}

//----------------------------------------------------------------//
float MOAIViewport::GetInvAspect () const {
	return this->Height () / this->Width ();
}

//----------------------------------------------------------------//
void MOAIViewport::GetNormToWndMtx ( USMatrix4x4& normToWnd ) const {

	USMatrix4x4 mtx;
	USRect rect = this->GetRect ();

	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Wnd
	normToWnd.Scale ( hWidth, -hHeight, 1.0f );
	
	mtx.Translate ( hWidth + rect.mXMin, hHeight + rect.mYMin, 0.0f );
	normToWnd.Append ( mtx );
}

//----------------------------------------------------------------//
USMatrix4x4 MOAIViewport::GetProjMtx () const {

	USMatrix4x4 proj;

	USMatrix4x4 mtx;
	USRect rect = this->GetRect ();

	// rotate
	proj.RotateZ ( -this->mRotation * ( float )D2R );

	// project
	USVec2D viewScale = this->GetScale ();
	float xScale = ( 2.0f / rect.Width ()) * viewScale.mX;
	float yScale = ( 2.0f / rect.Height ()) * viewScale.mY;
	
	mtx.Scale ( xScale, yScale, -1.0f );
	proj.Append ( mtx );
	
	// offset
	mtx.Translate ( this->mOffset.mX, this->mOffset.mY, 0.0f );
	proj.Append ( mtx );
	
	return proj;
}

//----------------------------------------------------------------//
USMatrix4x4 MOAIViewport::GetProjMtxInv () const {

	USMatrix4x4 projInv;

	USMatrix4x4 mtx;
	USRect rect = this->GetRect ();

	// Inv Project
	
	// offset
	projInv.Translate ( -this->mOffset.mX, -this->mOffset.mY, 0.0f );
	
	// project
	USVec2D viewScale = this->GetScale ();
	float invXScale = 1.0f / (( 2.0f / rect.Width () * viewScale.mX ));
	float invYScale = 1.0f / (( 2.0f / rect.Height () * viewScale.mY ));
	
	mtx.Scale ( invXScale, invYScale, -1.0f );
	projInv.Append ( mtx );
	
	// rotate
	mtx.RotateZ ( this->mRotation * ( float )D2R );
	projInv.Append ( mtx );
	
	return projInv;
}

//----------------------------------------------------------------//
USRect MOAIViewport::GetRect () const {

	return *this;
}

//----------------------------------------------------------------//
USVec2D MOAIViewport::GetScale () const {

	USVec2D scale ( 1.0f, 1.0f );
	
	if ( this->mXScale && this->mYScale ) {
		scale.mX = this->Width () / this->mScale.mX;
		scale.mY = this->Height () / this->mScale.mY;
	}
	else {
		
		float result = 1.0f;
		
		if ( this->mXScale ) {
			result = (( float )this->Width ()) / this->mScale.mX;
		}
		
		if ( this->mYScale ) {
			result = (( float )this->Height ()) / this->mScale.mY;
		}
		
		scale.mX = result;
		scale.mY = result;
	}
	
	return scale;
}

//----------------------------------------------------------------//
USVec2D MOAIViewport::GetUnits () const {

	USVec2D scale ( 1.0f, 1.0f );
	
	if ( this->mXScale && this->mYScale ) {
		scale.mX = this->mScale.mX;
		scale.mY = this->mScale.mY;
	}
	else {
		
		float result = 1.0f;
		
		if ( this->mXScale ) {
			result = this->mScale.mX;
		}
		
		if ( this->mYScale ) {
			result = this->mScale.mY;
		}
		
		scale.mX = result;
		scale.mY = result;
	}
	
	return scale;
}

//----------------------------------------------------------------//
void MOAIViewport::GetWndToNormMtx ( USMatrix4x4& wndToNorm ) const {

	USMatrix4x4 mtx;
	USRect rect = this->GetRect ();

	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Inv Wnd
	wndToNorm.Translate ( -hWidth - rect.mXMin, -hHeight - rect.mYMin, 0.0f );
	
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ), 1.0f );
	wndToNorm.Append ( mtx );
}

//----------------------------------------------------------------//
USMatrix4x4 MOAIViewport::GetWndToWorldMtx ( const USMatrix4x4& view ) const {

	USMatrix4x4 wndToWorld;

	USMatrix4x4 mtx;
	USRect rect = this->GetRect ();

	float hWidth = rect.Width () * 0.5f;
	float hHeight = rect.Height () * 0.5f;

	// Inv Wnd
	wndToWorld.Translate ( -hWidth - rect.mXMin, -hHeight - rect.mYMin, 0.0f );
	
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ), 1.0f );
	wndToWorld.Append ( mtx );

	mtx = this->GetProjMtxInv ();
	wndToWorld.Append ( mtx );
	
	mtx.Inverse ( view );
	wndToWorld.Append ( mtx );
	
	return wndToWorld;
}

//----------------------------------------------------------------//
USMatrix4x4 MOAIViewport::GetWorldToWndMtx ( const USMatrix4x4& view ) const {

	USMatrix4x4 worldToWnd;

	USMatrix4x4 mtx;
	USRect rect		= this->GetRect ();
	
	float hWidth	= this->Width () * 0.5f;
	float hHeight	= this->Height () * 0.5f;

	worldToWnd = view;

	mtx = this->GetProjMtx ();
	worldToWnd.Append ( mtx );

	// Wnd
	mtx.Scale ( hWidth, -hHeight, 1.0f );
	worldToWnd.Append ( mtx );
	
	mtx.Translate ( hWidth + rect.mXMin, hHeight + rect.mYMin, 0.0f );
	worldToWnd.Append ( mtx );
	
	return worldToWnd;
}

//----------------------------------------------------------------//
MOAIViewport::MOAIViewport () :
	mXScale ( true ),
	mYScale ( true ),
	mScale ( 1.0f, 1.0f ),
	mOffset ( 0.0f, 0.0f ),
	mRotation ( 0.0f ) {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	this->Init ( 0.0f, 0.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
MOAIViewport::~MOAIViewport () {
}

//----------------------------------------------------------------//
void MOAIViewport::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIViewport::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "setOffset",		_setOffset },
		{ "setRotation",	_setRotation },
		{ "setScale",		_setScale },
		{ "setSize",		_setSize },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIViewport::SetOffset ( float xOffset, float yOffset ) {

	this->mOffset.Init ( xOffset, yOffset );
}

//----------------------------------------------------------------//
void MOAIViewport::SetRotation ( float degrees ) {

	this->mRotation = degrees;
}

//----------------------------------------------------------------//
void MOAIViewport::SetScale ( float xScale, float yScale ) {

	this->mXScale = ( xScale != 0.0f );
	this->mScale.mX = xScale;
	
	this->mYScale = ( yScale != 0.0f );
	this->mScale.mY = yScale;
}
