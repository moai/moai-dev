// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com



#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIFreeTypeFont.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getDefaultSize
	@text	Requests the font's default size
 
	@in		MOAIFont self
	@out	float default size
 */

int MOAIFreeTypeFont::_getDefaultSize(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	state.Push( self->mDefaultSize );
	return 1;
}
//----------------------------------------------------------------//
/**	@name	getFilename
	@text	Returns the filename of the font.
 
	@in		MOAIFont self
	@out	name
 */

int MOAIFreeTypeFont::_getFilename(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	state.Push ( self->mFilename );
	return 1;
}
//----------------------------------------------------------------//
/**	@name	getFlags
	@text	Returns the current flags.
 
	@in		MOAIFont self
	@out	flags
 */
int MOAIFreeTypeFont::_getFlags(lua_State *L)
{
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	state.Push ( self->mFlags );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Sets the filename of the font for use when loading glyphs.
 
	@in		MOAIFont self
	@in		string filename			The path to the font file to load.
	@out	nil
 */

int	MOAIFreeTypeFont::_load(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "US" );
	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	self->Init ( filename );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setDefaultSize
	@text	Selects a glyph set size to use as the default size when no
			other size is specified by objects wishing to use MOAIFont to
			render text.
 
	@in		MOAIFont self
	@in		number points			The point size to be rendered onto the internal texture.
	@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
	@out	nil
 */

int MOAIFreeTypeFont::_setDefaultSize(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	
	float points	= state.GetValue < float >( 2, 0 );
	float dpi		= state.GetValue < float >( 3, DPI );
	
	self->mDefaultSize = POINTS_TO_PIXELS ( points, dpi );
	
	return 0;
}
//----------------------------------------------------------------//
/**	@name	setFlags
 @text	Set flags to control font loading behavior. Right now the
 only supported flag is FONT_AUTOLOAD_KERNING which may be used
 to enable automatic loading of kern tables. This flag is initially
 true by default.
 
 @in		MOAIFont self
 @opt	number flags			Flags are FONT_AUTOLOAD_KERNING or DEFAULT_FLAGS. DEFAULT_FLAGS is the same as FONT_AUTOLOAD_KERNING.
 Alternatively, pass '0' to clear the flags.
 @out	nil
 */

int MOAIFreeTypeFont::_setFlags(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	self->mFlags = state.GetValue < u32 >( 2, DEFAULT_FLAGS );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setReader
	@text	Attaches or clears the MOAIFontReader associated withthe font.
			MOAIFontReader is responsible for loading and rendering glyphs from
			a font file on demand. If you are using a static font and do not
			need a reader, set this field to nil.
 
	@in		MOAIFont self
	@opt	MOAIFontReader reader		Default value is nil.
	@out	nil
 */

int	MOAIFreeTypeFont::_setReader	( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	self->mReader.Set ( *self, state.GetLuaObject < MOAIFontReader >( 2, true ));
	return 0;
}

//================================================================//
// MOAIFreeTypeFont
//================================================================//


void MOAIFreeTypeFont::Init(cc8 *filename){
	if ( USFileSys::CheckFileExists ( filename, true )) {
		this->mFilename = USFileSys::GetAbsoluteFilePath ( filename );
	}
}

MOAIFreeTypeFont::MOAIFreeTypeFont():
	mFlags( DEFAULT_FLAGS ),
	mDefaultSize( 0.0f ){
	
	RTTI_BEGIN
		RTTI_EXTEND( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//

void MOAIFreeTypeFont::RegisterLuaClass ( MOAILuaState& state ) {
	state.SetField ( -1, "DEFAULT_FLAGS",			( u32 )DEFAULT_FLAGS );
	state.SetField ( -1, "FONT_AUTOLOAD_KERNING",	( u32 )FONT_AUTOLOAD_KERNING );
}

//----------------------------------------------------------------//

void MOAIFreeTypeFont::RegisterLuaFuncs(MOAILuaState &state){
	luaL_Reg regTable [] = {
		{ "load",						_load },
		{ "setReader",					_setReader },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
