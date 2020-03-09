// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIViewport.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIViewport::_getFrame ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIViewport, "U" )
	
	state.Push ( self->mXMin );
	state.Push ( self->mYMin );
	state.Push ( self->mXMax );
	state.Push ( self->mYMax );
	
	return 4;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIViewport::_getSize ( lua_State* L ) {
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
int MOAIViewport::_setOffset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIViewport, "UNN" )
	
	float xOffset = state.GetValue < float >( 2, 0.0f );
	float yOffset = state.GetValue < float >( 3, 0.0f );

	self->SetOffset ( xOffset, yOffset );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRotation
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
/**	@lua	setScale
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
/**	@lua	setSize
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
MOAIViewport::MOAIViewport () {
	
	MOAI_LUA_OBJECT_RTTI_SINGLE ( MOAIViewport, MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIViewport::~MOAIViewport () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIViewport::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIViewport::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getFrame",		_getFrame },
		{ "getSize",		_getSize },
		{ "setOffset",		_setOffset },
		{ "setRotation",	_setRotation },
		{ "setScale",		_setScale },
		{ "setSize",		_setSize },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
