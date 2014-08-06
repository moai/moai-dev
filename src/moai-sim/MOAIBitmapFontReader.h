// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBITMAPFONTREADER_H
#define	MOAIBITMAPFONTREADER_H

#include <moai-sim/MOAIFontReader.h>
#include <moai-sim/MOAIImage.h>

//================================================================//
// MOAIBitmapGlyph
//================================================================//
class MOAIBitmapGlyph {
private:
	
	friend class MOAIBitmapFontReader;
	friend class MOAIBitmapFontPage;
	
	bool			mIsWhitespace;
	ZLIntRect		mSrcRect;
	u32				mBase;
};

//================================================================//
// MOAIBitmapFontPage
//================================================================//
class MOAIBitmapFontPage {
private:

	friend class MOAIBitmapFontReader;
	
	typedef STLMap < u32, MOAIBitmapGlyph >::iterator BitmapGlyphsIt;
	STLMap < u32, MOAIBitmapGlyph > mBitmapGlyphs;
	
	MOAIImage mImage;
	
	u32 mBase;
	u32 mHeight;
	
	//----------------------------------------------------------------//
	void			GetGlyphBounds			( MOAIBitmapGlyph& bitmapGlyph, ZLIntRect& frame );
	ZLIntRect		GetGlyphFrame			( u32 x, u32 y, u32 maskColor );
	u32				GetRGB					( u32 x, u32 y );
	u32				GetRGBA					( u32 x, u32 y );
	void			RipBitmap				( cc8* filename, cc8* charCodes );

public:

	//----------------------------------------------------------------//
					MOAIBitmapFontPage		();
					~MOAIBitmapFontPage		();
};

//================================================================//
// MOAIBitmapFontReader
//================================================================//
/**	@lua	MOAIBitmapFontReader
	@text	<p>Legacy font reader for Moai's original bitmap font format.
			The original format is just a bitmap containing each glyph
			in the font divided by solid-color guide lines (see examples).
			This is an easy way for artists to create bitmap fonts. Kerning
			is not supported by this format.</p>
			
			<p>Runtime use of MOAIBitmapFontReader is not recommended. Instead,
			use MOAIBitmapFontReader as part of your tool chain to initialize
			a glyph cache and image to be serialized in later.</p>
			
*/
class MOAIBitmapFontReader :
	public MOAIFontReader {
private:

	typedef STLMap < float, MOAIBitmapFontPage >::iterator PagesIt;
	STLMap < float, MOAIBitmapFontPage > mPages;
	
	MOAIBitmapFontPage*		mCurrentPage;
	float					mCurrentPageSize;
	MOAIBitmapGlyph*		mCurrentGlyph;

	//----------------------------------------------------------------//
	static int		_loadPage					( lua_State* L );

	//----------------------------------------------------------------//
	void			LoadPage					( cc8* filename, float size, cc8* charCodes );

public:
	
	DECL_LUA_FACTORY ( MOAIBitmapFontReader )

	//----------------------------------------------------------------//
	int				GetFaceMetrics				( MOAIFontFaceMetrics& faceMetrics );
	int				GetGlyphMetrics				( MOAIGlyphMetrics& glyphMetrics );
					MOAIBitmapFontReader		();
					~MOAIBitmapFontReader		();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	int				RenderGlyph					( MOAIImage& image, float x, float y, const ZLColorBlendFunc& blendFunc );
	int				SelectFace					( float size );
	int				SelectGlyph					( u32 c );
	void			SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
