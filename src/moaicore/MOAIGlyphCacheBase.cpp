// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIGlyphCacheBase.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGlyphCacheBase::_getImage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGlyphCacheBase, "U" )

	MOAIImage* image = self->GetImage ();
	if ( image ) {
		state.Push ( image );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGlyphCacheBase::_setColorFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGlyphCacheBase, "UN" )

	self->mColorFormat = ( USColor::Format )state.GetValue < u32 >( 2, ( u32 )USColor::A_8 );

	return 0;	
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGlyphCacheBase::_setImage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGlyphCacheBase, "UU" )

	MOAIImage* image = state.GetLuaObject < MOAIImage >( 2 );
	if ( image ) {
		self->SetImage ( *image );
	}
	return 0;
}

//================================================================//
// MOAIGlyphCacheBase
//================================================================//

//----------------------------------------------------------------//
MOAIGlyphCacheBase::MOAIGlyphCacheBase () :
	mColorFormat ( USColor::A_8 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGlyphCacheBase::~MOAIGlyphCacheBase () {
}

//----------------------------------------------------------------//
void MOAIGlyphCacheBase::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIGlyphCacheBase::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getImage",				_getImage },
		{ "setColorFormat",			_setColorFormat },
		{ "setImage",				_setImage },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

