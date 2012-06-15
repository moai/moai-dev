// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAIVertexFormatMgr.h>

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
/**	@name	setStyle
	@text	Sets the particulars of a given debug line style.
	
	@in		number styleID		See MOAIDebugLines class documentation for a list of styles.
	@opt	number size			Pen size (in pixels) for the style. Default value is 1.
	@opt	number r			Red component of line color. Default value is 1.
	@opt	number g			Green component of line color. Default value is 1.
	@opt	number b			Blue component of line color. Default value is 1.
	@opt	number a			Alpha component of line color. Default value is 1.
	@out	nil
*/
int MOAIDebugLines::_setStyle ( lua_State* L ) {
	
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	u32 styleID		= state.GetValue < u32 >( 1, 0 );
	float size		= state.GetValue < float >( 2, 1.0f );
	float r			= state.GetValue < float >( 3, 1.0f );
	float g			= state.GetValue < float >( 4, 1.0f );
	float b			= state.GetValue < float >( 5, 1.0f );
	float a			= state.GetValue < float >( 6, 1.0f );
	
	u32 color = USColor::PackRGBA ( r, g, b, a );
	
	MOAIDebugLines::Get ().SetStyle ( styleID, size, color );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	showStyle
	@text	Enables of disables drawing of a given debug line style.
	
	@in		number styleID		See MOAIDebugLines class documentation for a list of styles.
	@opt	boolean show		Default value is 'true'
	@out	nil
*/
int MOAIDebugLines::_showStyle ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	u32 styleID		= state.GetValue < u32 >( 1, 0 );
	bool show		= state.GetValue < bool >( 2, true );
	
	MOAIDebugLines::Get ().ShowStyle ( styleID, show );
	
	return 0;
}

//================================================================//
// MOAIDebugLines
//================================================================//

//----------------------------------------------------------------//
bool MOAIDebugLines::Bind ( u32 styleID ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	MOAIDebugLineStyle& style = this->mStyles [ styleID ];
	
	if ( style.mVisible ) {
		gfxDevice.SetPenColor ( style.mColor );
		gfxDevice.SetPenWidth ( style.mSize );
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

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIDebugLines::~MOAIDebugLines () {
}

//----------------------------------------------------------------//
void MOAIDebugLines::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "setStyle",			_setStyle },
		{ "showStyle",			_showStyle },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
	
	state.SetField ( -1, "PARTITION_CELLS",			( u32 )PARTITION_CELLS );
	state.SetField ( -1, "PARTITION_PADDED_CELLS",	( u32 )PARTITION_PADDED_CELLS );
	state.SetField ( -1, "PROP_MODEL_BOUNDS",		( u32 )PROP_MODEL_BOUNDS );
	state.SetField ( -1, "PROP_WORLD_BOUNDS",		( u32 )PROP_WORLD_BOUNDS );
	state.SetField ( -1, "TEXT_BOX",				( u32 )TEXT_BOX );
	state.SetField ( -1, "TEXT_BOX_BASELINES",		( u32 )TEXT_BOX_BASELINES );
	state.SetField ( -1, "TEXT_BOX_LAYOUT",			( u32 )TEXT_BOX_LAYOUT );
}

//----------------------------------------------------------------//
void MOAIDebugLines::SetStyle ( u32 styleID, float size, u32 color ) {

	MOAIDebugLineStyle& style = this->mStyles [ styleID ];
	
	style.mVisible = true;
	style.mColor = color;
	style.mSize = size;
}

//----------------------------------------------------------------//
void MOAIDebugLines::ShowStyle ( u32 styleID, bool show ) {

	this->mStyles [ styleID ].mVisible = show;
}
