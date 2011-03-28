// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIViewport.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>() setOffset ( self, xOff, yOff )</tt>\n
\n
	Offsets the center of the viewport. Range is -1 to 1 along each axis.
	@param self (in)
	@param xOff (in)
	@param yOff (in)
*/
int MOAIViewport::_setOffset ( lua_State* L ) {
	LUA_SETUP ( MOAIViewport, "UNN" )
	
	float xOffset = state.GetValue < float >( 2, 0.0f );
	float yOffset = state.GetValue < float >( 3, 0.0f );

	self->SetOffset ( xOffset, yOffset );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setRect ( self, left, top, right, bottom )</tt>\n
	\n
	Set viewport rectangle in window space.
	@param self (in)
	@param left (in)
	@param top (in)
	@param right (in)
	@param bottom (in)
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIViewport::_setRotation ( lua_State* L ) {
	LUA_SETUP ( MOAIViewport, "U" )
	
	float rotation = state.GetValue < float >( 2, 0.0f );
	self->SetRotation ( rotation );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setScale ( self, xScale, yScale )</tt>\n
	\n
	Sets the number of world units visible of the viewport for one or both dimensions. Set '0' to use derived value.
	@param self (in)
	@param xScale (in) Total number of world units visible along width of viewport or 0.
	@param yScale (in) Total number of world units visible along height of viewport or 0.
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
/**	@brief <tt>setSize ( self, width, height )</tt>\n
	\n
	Set viewport rectangle from width and height. (Viewport will align to upper left corner of window.)
	@param self (in)
	@param width (in) Width of viewport in window space.
	@param height (in) Height of viewport in window space.
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
