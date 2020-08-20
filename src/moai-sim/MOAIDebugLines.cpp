// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>

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
MOAIDebugLineStyleSet::MOAIDebugLineStyleSet () :
	mShowDebugLines ( true ) {
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

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDebugLinesMgr::_showDebugLines ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIDebugLinesMgr, "" )

	if ( state.IsType ( 1, LUA_TNUMBER )) {
	
		u32 packedID = state.GetValue < u32 >( 1, 0 );
		
		MOAIDebugLineStyleSet* styleSet = self->mStyleSets.value_for_key ( MOAIDebugLinesMgr::GetSetID ( packedID ), 0 );
		if ( styleSet ) {
			styleSet->mShowDebugLines = state.GetValue < bool >( 2, true );
		}
	}
	else {
		self->mShowDebugLines = state.GetValue < bool >( 1, true );
	}
	return 0;
}

//================================================================//
// MOAIDebugLinesMgr
//================================================================//

//----------------------------------------------------------------//
bool MOAIDebugLinesMgr::Bind ( u32 styleID ) {

	return this->Bind ( styleID, MOAIDraw::Get ());
}

//----------------------------------------------------------------//
bool MOAIDebugLinesMgr::Bind ( u32 styleID, MOAIDrawAPI& draw ) {

	if ( !this->mShowDebugLines ) return false;

	MOAIDebugLineStyle* style = this->GetStyle ( styleID );
	if ( style && ( style->mDisplay == MOAIDebugLineStyle::DISPLAY_VISIBLE )) {

		draw.SetPenColor ( style->mColor );
		draw.SetPenWidth ( style->mSize );
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
bool MOAIDebugLinesMgr::IsVisible () {

	return this->mShowDebugLines;
}

//----------------------------------------------------------------//
bool MOAIDebugLinesMgr::IsVisible ( u32 styleID ) {

	if ( !this->mShowDebugLines ) return false;
	
	MOAIDebugLineStyle* style = this->GetStyle ( styleID );
	if ( style ) {
		return style->mDisplay == MOAIDebugLineStyle::DISPLAY_VISIBLE;
	}
	return false;
}

//----------------------------------------------------------------//
MOAIDebugLinesMgr::MOAIDebugLinesMgr () :
	mActiveStyleSet ( 0 ),
	mShowDebugLines ( true ) {
	
	RTTI_BEGIN ( MOAIDebugLinesMgr )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIDebugLinesMgr >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDebugLinesMgr::~MOAIDebugLinesMgr () {

	StyleSetIt styleSetIt = this->mStyleSets.begin ();
	for ( ; styleSetIt != this->mStyleSets.end (); ++styleSetIt ) {
		delete ( styleSetIt->second );
	}
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
bool MOAIDebugLinesMgr::SelectStyleSet ( u32 setID ) {

	this->mActiveStyleSet = this->mStyleSets.value_for_key ( setID, 0 );
	return this->mActiveStyleSet && this->mActiveStyleSet->mShowDebugLines;
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

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDebugLinesMgr::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable[] = {
		{ "setStyle",			_setStyle },
		{ "showDebugLines",		_showDebugLines },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
