// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIViewport.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIViewport::_getViewFrame ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIViewport, "U" )
	
	state.Push ( self->mXMin );
	state.Push ( self->mYMin );
	state.Push ( self->mXMax );
	state.Push ( self->mYMax );
	
	return 4;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIViewport::_getViewSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIViewport, "U" )
	
	state.Push ( self->Width ());
	state.Push ( self->Height ());
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	setOffset
	@text	Sets the viewport offset in normalized view space (size of
			viewport is -1 to 1 in both directions).
	
	@in		MOAIViewport self
	@in		number xOff
	@in		number yOff
	@out	nil
*/
int MOAIViewport::_setViewOffset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIViewport, "UNN" )
	
	float xOffset = state.GetValue < float >( 2, 0.0f );
	float yOffset = state.GetValue < float >( 3, 0.0f );

	self->SetOffset ( xOffset, yOffset );

	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
/**	@lua	setViewRotation
	@text	Sets global rotation to be added to camera transform.
	
	@in		MOAIViewport self
	@in		number rotation
	@out	nil
*/
int MOAIViewport::_setViewRotation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIViewport, "U" )
	
	float rotation = state.GetValue < float >( 2, 0.0f );
	self->SetRotation ( rotation );
	
	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
/**	@lua	setViewScale
	@text	Sets the number of world units visible of the viewport for one or both
			dimensions. Set 0 for one of the dimensions to use a derived value based on
			the other dimension and the aspect ratio. Negative values are also OK.
			
			Note that the while the contents of the viewport will appear to stretch
			or shrink to match the dimensions of the viewport given by setSize, the
			number of world units visible will remain constant.
	
	@in		MOAIViewport self
	@in		number xScale
	@in		number yScale
	@out	nil
*/
int MOAIViewport::_setViewScale ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UNN" )) return 0;
	
	MOAIViewport* self = state.GetLuaObject < MOAIViewport >( 1, true );
	if ( !self ) return 0;

	float xScale = state.GetValue < float >( 2, 0.0f );
	float yScale = state.GetValue < float >( 3, 0.0f );
	
	self->SetScale ( xScale, yScale );

	MOAI_LUA_RETURN_SELF
}

//----------------------------------------------------------------//
/**	@lua	setViewSize
	@text	Sets the dimensions of the viewport.
	
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
int MOAIViewport::_setViewSize ( lua_State* L ) {

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

	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIViewport
//================================================================//

//----------------------------------------------------------------//
ZLViewport MOAIViewport::GetWorldViewport () const {

	ZLViewport viewport = *this;
	ZLMatrix4x4 mtx ( this->mLocalToWorldMtx );
	mtx.Transform ( viewport );
	return viewport;
}

//----------------------------------------------------------------//
MOAIViewport::MOAIViewport () {
		
	RTTI_BEGIN ( MOAIViewport )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIViewport >)
		RTTI_EXTEND ( MOAITransform )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIViewport::~MOAIViewport () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIViewport::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIViewport::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getViewFrame",		_getViewFrame },
		{ "getViewSize",		_getViewSize },
		{ "setViewOffset",		_setViewOffset },
		{ "setViewRotation",	_setViewRotation },
		{ "setViewScale",		_setViewScale },
		{ "setViewSize",		_setViewSize },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
