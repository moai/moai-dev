// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFREETYPEFONT_H
#define MOAIFREETYPEFONT_H


#include <moaicore/MOAILua.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <vector>

class MOAIFontReader;
class MOAITexture;

#define DPI 72
#define POINTS_TO_PIXELS(points,dpi) (( points * dpi ) / DPI )
#define PIXELS_TO_POINTS(pixels,dpi) (( pixels * DPI ) / dpi )


struct MOAIFreeTypeTextLine {
	int lineWidth;
	u32 startIndex;
	u32* text;
};

//================================================================//
// MOAIFreeTypeFont
//================================================================//

/**	@name	MOAIFreeTypeFont
	@text	A replacement of MOAIFont that does not use MOAIGlyph or MOAIGlyphSet.
			
 
 */


class MOAIFreeTypeFont : public virtual MOAILuaObject {
protected:
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
	
	std::vector<MOAIFreeTypeTextLine> mLineVector;
		
	//----------------------------------------------------------------//
	static int			_dimensionsOfLine		( lua_State* L );
	static int			_getDefaultSize         ( lua_State* L );
	static int			_getFilename			( lua_State* L );
	static int			_getFlags				( lua_State* L );
	static int			_load					( lua_State* L );
	static int			_optimalSize			( lua_State* L );
	static int			_renderTexture			( lua_State* L );
	static int			_renderTextureSingleLine( lua_State* L );
	static int			_setDefaultSize			( lua_State* L );
	static int			_setFlags				( lua_State* L );
	static int			_setReader				( lua_State* L );
	
		
	//----------------------------------------------------------------//
	void				BuildLine				(u32* buffer, size_t buf_len, int pen_x,
												 u32 lastChar, u32 startIndex);
	void				BuildLine				(u32* buffer, size_t bufferLength, u32 startIndex);
	USRect				DimensionsOfLine		(cc8* text, float fontSize, FT_Vector **glyphPositions,
												 FT_Glyph **glyphArray, FT_UInt *glyphNumber, FT_Int *maxDescender,
												 FT_Int *maxAscender);
	int					ComputeLineStart		(FT_UInt unicode, int lineIndex,
												 int alignment, FT_Int imgWidth);
	int					ComputeLineStartY		(int textHeight, FT_Int imgHeight, int vAlign);
	void				DrawBitmap				(FT_Bitmap* bitmap, FT_Int x, FT_Int y, FT_Int imgWidth,
												 FT_Int imgHeight);
	void				GenerateLines			( FT_Int imgWidth, cc8* text, int wordBreak);
	void				InitBitmapData			( u32 width, u32 height );
	int					NumberOfLinesToDisplayText(cc8* text, FT_Int imageWidth, int wordBreakMode, bool generateLines);
	void				RenderLines				( FT_Int imgWidth, FT_Int imgHeight, int hAlign,
												 int vAlign, bool returnGlyphBounds,
												 MOAILuaState& state);
	void				ResetBitmapData			();
	int					WidthOfString			(u32* buffer, size_t bufferLength);
		
public:
		
	DECL_LUA_FACTORY ( MOAIFreeTypeFont )
		
	GET ( cc8*, Filename, mFilename );
	GET ( float, DefaultSize, mDefaultSize );
	GET ( FT_Face, FreeTypeFace, mFreeTypeFace);
	
	//----------------------------------------------------------------//
		
	USRect				DimensionsOfLine		(cc8* text, float fontSize);
	void				Init					( cc8* filename );
	static bool			IsWordBreak				(u32 character, int wordBreakMode);
	static u32			LengthOfUTF8Sequence	(const u32 *sequence);
	FT_Face				LoadFreeTypeFace		(FT_Library *library);
						MOAIFreeTypeFont        ();
						~MOAIFreeTypeFont		();
	float				OptimalSize				(cc8* text, float width, float height,
												 float maxFontSize, float minFontSize,
												 int wordbreak, bool forceSingleLine );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	MOAITexture*		RenderTexture			( cc8* text, float size, float width,
												 float height, int hAlignment, int vAlignment,
												 int wordbreak, bool autoFit, bool returnGlyphBounds,
												 MOAILuaState& state);
	MOAITexture*		RenderTextureSingleLine ( cc8* text, float fontSize, USRect *rect,
												 bool returnGlyphBounds, MOAILuaState& state );
	static u32			WideCharStringLength	(u32 *string);
	
	
};


#endif /* defined(MOAIFREETYPEFONT_H) */
