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
/**	@name	getImage
	@text	Returns an image representing a combination of all of the
			texture pages allocated by the cache. In some implementaions of
			static caches this method may not be available.
	
	@in		MOAIFont self
	@out	MOAIImage image
*/
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

//----------------------------------------------------------------//
/**	@name	setImage
	@text	Passes an image to the glyph cache that will be used to
			recreate and initialize its texture memory. It will not
			affect any glyph entires that have already been laid
			out and stored in the cache. In some implementaions of
			dynamic caches this method may not be available.
	
	@in		MOAIFont self
	@in		MOAIImage image
	@out	nil
*/
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

