// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
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
	LUA_SETUP ( MOAIViewport, "UNN" )
	
	float xOffset = state.GetValue < float >( 2, 0.0f );
	float yOffset = state.GetValue < float >( 3, 0.0f );

	self->SetOffset ( xOffset, yOffset );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRect
	@text	Sets the viewport rectangle in screen space.
	
	@in		MOAIViewport self
	@in		number left
	@in		number top
	@in		number right
	@in		number bottom
	@out	nil
*/
int MOAIViewport::_setRect ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UNNNN" )) return 0;
	
	MOAIViewport* self = state.GetLuaObject < MOAIViewport >( 1 );
	if ( !self ) return 0;

	float left		= state.GetValue < float >( 2, 0.0f );
	float top		= state.GetValue < float >( 3, 0.0f );
	float right		= state.GetValue < float >( 4, 0.0f );
	float bottom	= state.GetValue < float >( 5, 0.0f );
	
	self->Init ( left, top, right, bottom );

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
	LUA_SETUP ( MOAIViewport, "U" )
	
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
			viewport. This practice is neither endorsed nor condemned.
			
			Note that the while the contents of the viewport will appear to stretch
			or shrink to match the dimensions of the viewport given by setRect, the
			number of world units visible will remain constant.
	
	@in		MOAIViewport self
	@in		number xScale
	@in		number yScale
	@out	nil
*/
int MOAIViewport::_setScale ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UNN" )) return 0;
	
	MOAIViewport* self = state.GetLuaObject < MOAIViewport >( 1 );
	if ( !self ) return 0;

	float xScale = state.GetValue < float >( 2, 0.0f );
	float yScale = state.GetValue < float >( 3, 0.0f );
	
	self->SetScale ( xScale, yScale );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSize
	@text	Equivalent to setRect ( 0, 0, width, height )
	
	@in		MOAIViewport self
	@in		number width
	@in		number height
	@out	nil
*/
int MOAIViewport::_setSize ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UNN" )) return 0;
	
	MOAIViewport* self = state.GetLuaObject < MOAIViewport >( 1 );
	if ( !self ) return 0;

	float width = state.GetValue < float >( 2, 0.0f );
	float height = state.GetValue < float >( 3, 0.0f );
	
	self->Init ( 0.0f, 0.0f, width, height );

	return 0;
}

//================================================================//
// MOAIViewport
//================================================================//

//----------------------------------------------------------------//
MOAIViewport::MOAIViewport () {
	
	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIViewport::~MOAIViewport () {
}

//----------------------------------------------------------------//
void MOAIViewport::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIViewport::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "setOffset",		_setOffset },
		{ "setRect",		_setRect },
		{ "setRotation",	_setRotation },
		{ "setScale",		_setScale },
		{ "setSize",		_setSize },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIViewport::ToString () {

	// TODO pretty print rects!

	STLString repr;

	PRETTY_PRINT ( repr, GetScale () )

	return repr;
}
