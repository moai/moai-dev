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
/**	@name	setColorFormat
	@text	The color format may be used by dynamic cache implementations
			when allocating new textures.
	
	@in		MOAIFont self
	@in		number colorFmt		One of MOAIImage.COLOR_FMT_A_8, MOAIImage.COLOR_FMT_RGB_888, MOAIImage.COLOR_FMT_RGB_565,
								MOAIImage.COLOR_FMT_RGBA_5551, MOAIImage.COLOR_FMT_RGBA_4444, COLOR_FMT_RGBA_8888
	@out	nil
*/
int MOAIGlyphCacheBase::_setColorFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGlyphCacheBase, "UN" )

	self->mColorFormat = ( USColor::Format )state.GetValue < u32 >( 2, ( u32 )USColor::A_8 );

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
		{ "setColorFormat",			_setColorFormat },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

