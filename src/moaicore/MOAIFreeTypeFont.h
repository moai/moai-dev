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

struct MOAIOptimalSizeParameters {
	cc8* text;
	float width;
	float height;
	float maxFontSize;
	float minFontSize;
	int wordBreak;
	bool forceSingleLine;
	float granularity;
	bool roundToInteger;
	float lineSpacing;
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
	float mDefaultSize;
		
	FT_Face mFreeTypeFace;
	
	unsigned char* mBitmapData;
	u32 mBitmapWidth;
	u32 mBitmapHeight;
	
	std::vector<MOAIFreeTypeTextLine> mLineVector;
	
	FT_Glyph* mGlyphArray;
	FT_Vector* mAdvanceArray;
		
	//----------------------------------------------------------------//
	static int			_dimensionsOfLine		( lua_State* L );
	static int			_dimensionsWithMaxWidth	( lua_State* L );
	static int			_getDefaultSize         ( lua_State* L );
	static int			_getFilename			( lua_State* L );
	static int			_load					( lua_State* L );
	static int			_optimalSize			( lua_State* L );
	static int			_renderTexture			( lua_State* L );
	static int			_renderTextureSingleLine( lua_State* L );
	static int			_setDefaultSize			( lua_State* L );
		
	//----------------------------------------------------------------//
	void				BuildLine				(u32* buffer, size_t buf_len, int pen_x,
												 u32 lastChar, u32 startIndex);
	void				BuildLine				(u32* buffer, size_t bufferLength, u32 startIndex);
	int					ComputeLineStart		(FT_UInt unicode, size_t lineIndex,
												 int alignment, FT_Int imageWidth);
	int					ComputeLineStartY		(int textHeight, FT_Int imageHeight, int vAlign);
	USRect				DimensionsOfLine		(cc8* text, float fontSize, FT_Vector **glyphPositions,
												 FT_Glyph **glyphArray, FT_UInt *glyphNumber, size_t maxGlyphs, FT_Int *maxDescender,
												 FT_Int *maxAscender);
	USRect				DimensionsWithMaxWidth	(cc8* text, float fontSize, float width, int wordBreak, bool returnGlyphBounds,
												 float lineSpacing, MOAILuaState& state);
	void				DrawBitmap				(FT_Bitmap* bitmap, FT_Int x, FT_Int y, FT_Int imageWidth,
												 FT_Int imageHeight);
	void				GenerateLines			( FT_Int imageWidth, cc8* text, int wordBreak);
	void				InitBitmapData			( u32 width, u32 height );
	static int			NewPropFromFittedTexture( MOAILuaState& state, bool singleLine);
	static void			PushRectAndBaselineToLuaTable	(USRect rect, int *baseline, u32 index, MOAILuaState &state, u32 wideChar = 0);
	void				RenderLines				( FT_Int imageWidth, FT_Int imageHeight, int hAlign,
												 int vAlign, bool returnGlyphBounds, float lineSpacing,
												 MOAILuaState& state);
	void				ResetBitmapData			();
	void				StoreGlyphAndAdvanceAtIndex		(size_t index);
	int					WidthOfString			(u32* buffer, size_t bufferLength, u32 startIndex = 0);
	
	
	
public:
		
	DECL_LUA_FACTORY ( MOAIFreeTypeFont )
		
	GET ( cc8*, Filename, mFilename );
	GET ( float, DefaultSize, mDefaultSize );
	GET ( FT_Face, FreeTypeFace, mFreeTypeFace);
	
	//----------------------------------------------------------------//
	FT_Face				AffirmFreeTypeFace		();
	USRect				DimensionsOfLine		(cc8* text, float fontSize, bool returnGlyphBounds,
												 MOAILuaState& state);
	float				EstimatedMaxFontSize	(float height, float inputSize);
	FT_Int				GetLineHeight			();
	int					GetMaxLinesInArea		(u32 lineHeight, float lineSpacing, bool forceSingleLine, float areaHeight);
	bool				IsTextLargerThanArea	(cc8* text, FT_Int imageWidth, int wordBreakMode,
												 float lineSpacing, bool forceSingleLine, float areaHeight);
	void				Init					( cc8* filename );
	bool				IsFreeTypeInitialized	();
	static bool			IsWordBreak				(u32 character, int wordBreakMode);
	static u32			LengthOfUTF8Sequence	(const u32 *sequence);
	FT_Face				LoadFreeTypeFace		(FT_Library *library);
						MOAIFreeTypeFont        ();
						~MOAIFreeTypeFont		();
	int					NumberOfLinesToDisplayText(cc8* text, FT_Int imageWidth, int wordBreakMode, bool generateLines);
	float				OptimalSize				(const MOAIOptimalSizeParameters& params );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	MOAITexture*		RenderTexture			( cc8* text, float size, float width,
												 float height, int hAlignment, int vAlignment,
												 int wordbreak, bool autoFit, bool returnGlyphBounds,
												 float lineSpacing, MOAILuaState& state);
	MOAITexture*		RenderTextureSingleLine ( cc8* text, float fontSize, USRect *rect,
												 bool returnGlyphBounds, MOAILuaState& state );
	void				SetCharacterSize		(float fontSize);
	static u32			WideCharStringLength	(u32 *string);
	
	
};


#endif /* defined(MOAIFREETYPEFONT_H) */
