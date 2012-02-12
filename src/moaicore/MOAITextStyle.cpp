// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAINodeMgr.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAITextStyle.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAITextStyle::_setFont ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "U" )
	return 0;
}

//================================================================//
// MOAITextStyle
//================================================================//

//----------------------------------------------------------------//
MOAITextStyle::MOAITextStyle () :
	mPoints ( 1.0f ),
	mColor ( 0xffffffff ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITextStyle::~MOAITextStyle () {

	this->mFont.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAITextStyle::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAITextStyle::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
	
	luaL_Reg regTable [] = {
		{ "setFont",				_setFont },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITextStyle::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAITextStyle::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAITextStyle::SetFont ( MOAIFont* font ) {

	this->mFont.Set ( *this, font );
}