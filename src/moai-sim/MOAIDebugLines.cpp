// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// MOAIDebugLineStyle
//================================================================//

//----------------------------------------------------------------//
MOAIDebugLineStyle::MOAIDebugLineStyle () :
	mDisplay ( DISPLAY_PASS ),
	mSize ( 1.0f ),
	mColor ( 0xffffffff ) {
}

//----------------------------------------------------------------//
MOAIDebugLineStyle::~MOAIDebugLineStyle () {
}

//================================================================//
// MOAIDebugLineStyleSet
//================================================================//

//----------------------------------------------------------------//
MOAIDebugLineStyle* MOAIDebugLineStyleSet::GetStyle ( u32 styleID ) {

	return styleID < this->mLineStyles.Size () ? &this->mLineStyles [ styleID ] : 0;
}

//----------------------------------------------------------------//
MOAIDebugLineStyleSet::MOAIDebugLineStyleSet () {
}

//----------------------------------------------------------------//
MOAIDebugLineStyleSet::~MOAIDebugLineStyleSet () {
}

//----------------------------------------------------------------//
void MOAIDebugLineStyleSet::ReserveStyles ( u32 size ) {

	this->mLineStyles.Init ( size );
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setStyle
	@text	Sets the particulars of a given debug line style.
	
	@in		number styleID		See MOAIDebugLinesMgr class documentation for a list of styles.
	@opt	number size			Pen size (in pixels) for the style. Default value is 1.
	@opt	number r			Red component of line color. Default value is 1.
	@opt	number g			Green component of line color. Default value is 1.
	@opt	number b			Blue component of line color. Default value is 1.
	@opt	number a			Alpha component of line color. Default value is 1.
	@out	nil
*/
int MOAIDebugLinesMgr::_setStyle ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDebugLinesMgr, "" )
	
	u32 packedID	= state.GetValue < u32 >( 1, 0 );
	float size		= state.GetValue < float >( 2, 1.0f );
	float r			= state.GetValue < float >( 3, 1.0f );
	float g			= state.GetValue < float >( 4, 1.0f );
	float b			= state.GetValue < float >( 5, 1.0f );
	float a			= state.GetValue < float >( 6, 1.0f );
	
	u32 color = ZLColor::PackRGBA ( r, g, b, a );
	
	self->SelectStyleSet ( MOAIDebugLinesMgr::GetSetID ( packedID ));
	self->SetStyle ( MOAIDebugLinesMgr::GetStyleID ( packedID ), size, color );
	
	return 0;
}

//================================================================//
// MOAIDebugLinesMgr
//================================================================//

//----------------------------------------------------------------//
bool MOAIDebugLinesMgr::Bind ( u32 styleID ) {

	MOAIDebugLineStyle* style = this->GetStyle ( styleID );
	if ( style && ( style->mDisplay == MOAIDebugLineStyle::DISPLAY_VISIBLE )) {

		MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

		gfxMgr.mGfxState.SetPenColor ( style->mColor );
		gfxMgr.mGfxState.SetPenWidth ( style->mSize );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
u32 MOAIDebugLinesMgr::GetSetID ( u32 packedID ) {

	return ( packedID & STYLE_SET_ID_MASK ) >> STYLE_SET_ID_SHIFT;
}

//----------------------------------------------------------------//
MOAIDebugLineStyle* MOAIDebugLinesMgr::GetStyle ( u32 styleID ) {

	return this->mActiveStyleSet ? this->mActiveStyleSet->GetStyle ( styleID ) : 0;
}

//----------------------------------------------------------------//
u32 MOAIDebugLinesMgr::GetStyleID ( u32 packedID ) {

	return packedID & STYLE_ID_MASK;
}

//----------------------------------------------------------------//
bool MOAIDebugLinesMgr::IsVisible ( u32 styleID ) {

	MOAIDebugLineStyle* style = this->GetStyle ( styleID );
	if ( style ) {
		return style->mDisplay == MOAIDebugLineStyle::DISPLAY_VISIBLE;
	}
}

//----------------------------------------------------------------//
MOAIDebugLinesMgr::MOAIDebugLinesMgr () :
	mActiveStyleSet ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIDebugLinesMgr::~MOAIDebugLinesMgr () {

	StyleSetIt styleSetIt = this->mStyleSets.begin ();
	for ( ; styleSetIt != this->mStyleSets.end (); ++styleSetIt ) {
		delete ( styleSetIt->second );
	}
}

//----------------------------------------------------------------//
void MOAIDebugLinesMgr::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "setStyle",			_setStyle },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDebugLinesMgr::ReserveStyleSet ( u32 setID, u32 size ) {

	MOAIDebugLineStyleSet* styleSet = this->mStyleSets.value_for_key ( setID, 0 );
	if ( !styleSet ) {
	
		styleSet = new MOAIDebugLineStyleSet ();
		this->mStyleSets [ setID ] = styleSet;
	}
	styleSet->ReserveStyles ( size );
	this->mActiveStyleSet = styleSet;
}

//----------------------------------------------------------------//
void MOAIDebugLinesMgr::SelectStyleSet ( u32 setID ) {

	this->mActiveStyleSet = this->mStyleSets.value_for_key ( setID, 0 );
}

//----------------------------------------------------------------//
void MOAIDebugLinesMgr::SetStyle ( u32 styleID ) {

	MOAIDebugLineStyle* style = this->GetStyle ( styleID );
	if ( style ) {
		style->mDisplay = MOAIDebugLineStyle::DISPLAY_PASS;
	}
}

//----------------------------------------------------------------//
void MOAIDebugLinesMgr::SetStyle ( u32 styleID, float size, u32 color ) {

	MOAIDebugLineStyle* style = this->GetStyle ( styleID );
	if ( style ) {
		style->mDisplay = size > 0.0f ? MOAIDebugLineStyle::DISPLAY_VISIBLE : MOAIDebugLineStyle::DISPLAY_HIDE;
		style->mSize = size;
		style->mColor = color;
	}
}
