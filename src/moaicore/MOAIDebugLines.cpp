// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDebugLines.h>

//================================================================//
// MOAIDebugLineStyle
//================================================================//

//----------------------------------------------------------------//
MOAIDebugLineStyle::MOAIDebugLineStyle () :
	mVisible ( false ),
	mColor ( 0xffffffff ),
	mSize ( 1 ) {
}

//----------------------------------------------------------------//
MOAIDebugLineStyle::~MOAIDebugLineStyle () {
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIDebugLines::_setStyle ( lua_State* L ) {
	
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "NNNNN" )) return 0;
	
	u32 styleID		= state.GetValue < u32 >( 1, 0 );
	u32 size		= state.GetValue < u32 >( 2, 1 );
	float r			= state.GetValue < float >( 3, 1.0f );
	float g			= state.GetValue < float >( 4, 1.0f );
	float b			= state.GetValue < float >( 5, 1.0f );
	float a			= state.GetValue < float >( 6, 1.0f );
	
	u32 color = USColor::PackRGBA ( r, g, b, a );
	
	MOAIDebugLines::Get ().SetStyle ( styleID, size, color );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIDebugLines::_showStyle ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "NB" )) return 0;
	
	u32 styleID		= state.GetValue < u32 >( 1, 0 );
	bool show		= state.GetValue < bool >( 2, false );
	
	MOAIDebugLines::Get ().ShowStyle ( styleID, show );
	
	return 0;
}

//================================================================//
// MOAIDebugLines
//================================================================//

//----------------------------------------------------------------//
bool MOAIDebugLines::Bind ( u32 styleID ) {

	MOAIDebugLineStyle& style = this->mStyles [ styleID ];
	
	if ( style.mVisible ) {
		this->SetPenColor ( style.mColor );
		this->SetPenWidth ( style.mSize );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIDebugLines::IsVisible ( u32 styleID ) {

	return this->mStyles [ styleID ].mVisible;
}

//----------------------------------------------------------------//
MOAIDebugLines::MOAIDebugLines () {

	RTTI_SINGLE ( USLuaData )
}

//----------------------------------------------------------------//
MOAIDebugLines::~MOAIDebugLines () {
}

//----------------------------------------------------------------//
void MOAIDebugLines::RegisterLuaClass ( USLuaState& state ) {

	LuaReg regTable[] = {
		{ "setStyle",			_setStyle },
		{ "showStyle",			_showStyle },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
	
	state.SetField ( -1, "COLLISION_PRIMS",			( u32 )COLLISION_PRIMS );
	state.SetField ( -1, "OVERLAPS",				( u32 )OVERLAPS );
	state.SetField ( -1, "PARTITION_CELLS",			( u32 )PARTITION_CELLS );
	state.SetField ( -1, "PARTITION_PADDED_CELLS",	( u32 )PARTITION_PADDED_CELLS );
	state.SetField ( -1, "SURFACES",				( u32 )SURFACES );
	state.SetField ( -1, "SPRITE_MODEL_BOUNDS",		( u32 )SPRITE_MODEL_BOUNDS );
	state.SetField ( -1, "SPRITE_WORLD_BOUNDS",		( u32 )SPRITE_WORLD_BOUNDS );
	state.SetField ( -1, "TEXT_BOX",				( u32 )TEXT_BOX );
	state.SetField ( -1, "TOTAL_STYLES",			( u32 )TOTAL_STYLES );
}

//----------------------------------------------------------------//
void MOAIDebugLines::SetStyle ( u32 styleID, u32 size, u32 color ) {

	MOAIDebugLineStyle& style = this->mStyles [ styleID ];
	
	style.mVisible = true;
	style.mColor = color;
	style.mSize = size;
}

//----------------------------------------------------------------//
void MOAIDebugLines::ShowStyle ( u32 styleID, bool show ) {

	this->mStyles [ styleID ].mVisible = show;
}
