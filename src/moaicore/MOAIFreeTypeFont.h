// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFREETYPEFONT_H
#define MOAIFREETYPEFONT_H


#include <moaicore/MOAILua.h>
#include <ft2build.h>
#include FT_FREETYPE_H

class MOAIFontReader;
class MOAIFreeTypeTextBox;
class MOAITexture;

#define DPI 72
#define POINTS_TO_PIXELS(points,dpi) (( points * dpi ) / DPI )
#define PIXELS_TO_POINTS(pixels,dpi) (( pixels * DPI ) / dpi )

//================================================================//
// MOAIFreeTypeFont
//================================================================//

/**	@name	MOAIFreeTypeFont
	@text	A replacement of MOAIFont that does not use MOAIGlyph or MOAIGlyphSet.
			
 
 */


class MOAIFreeTypeFont : public virtual MOAILuaObject {
protected:
	friend class MOAIFreeTypeTextBox;

	STLString mFilename;
	u32 mFlags;
		
	MOAILuaSharedPtr < MOAIFontReader > mReader;
		
	float mDefaultSize;
		
	enum {
		FONT_AUTOLOAD_KERNING		= 0x01,
	};
	
	static const u32 DEFAULT_FLAGS = FONT_AUTOLOAD_KERNING;
		
	FT_Face mFreeTypeFace;
	
	
	unsigned char* mBitmapData;
	u32 mBitmapWidth;
	u32 mBitmapHeight;
		
	//----------------------------------------------------------------//
	static int			_getDefaultSize         ( lua_State* L );
	static int			_getFilename			( lua_State* L );
	static int			_getFlags				( lua_State* L );
	static int			_load					( lua_State* L );
	static int			_optimalSize			( lua_State* L );
	static int			_renderTexture			( lua_State* L );
	static int			_setDefaultSize			( lua_State* L );
	static int			_setFlags				( lua_State* L );
	static int			_setReader				( lua_State* L );
	
		
	//----------------------------------------------------------------//
	void				InitBitmapData			( u32 width, u32 height ); 
		
public:
		
	DECL_LUA_FACTORY ( MOAIFreeTypeFont )
		
	GET ( cc8*, Filename, mFilename );
	GET ( float, DefaultSize, mDefaultSize );
	GET ( FT_Face, FreeTypeFace, mFreeTypeFace);
	
	//----------------------------------------------------------------//
		
	void				Init					( cc8* filename );
	FT_Face				LoadFreeTypeFace		(FT_Library *library);
						MOAIFreeTypeFont        ();
						~MOAIFreeTypeFont		();
	float				OptimalSize				(cc8* text, float width, float height, float maxFontSize, float minFontSize, int wordbreak, bool forceSingleLine );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	MOAITexture*		RenderTexture			( cc8* text, float size, float width, float height, int hAlignment, int vAlignment, int wordbreak, bool autoFit );

};


#endif /* defined(MOAIFREETYPEFONT_H) */
