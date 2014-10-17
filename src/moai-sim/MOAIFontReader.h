// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONTREADER_H
#define	MOAIFONTREADER_H

class MOAIImage;
class MOAIKernVec;

// we keep font face and glyph dimensions in pixels rather than UVs to make it easier to generate the
// screen rect for the glyph and also to keep the glyph's UV bounds from being invalidated
// if the texture page gets resized to make room for more glyphs

//================================================================//
// MOAIKernVec
//================================================================//
class MOAIKernVec :
	public ZLVec2D {
public:

	u32			mName;
};

//================================================================//
// MOAIFontFaceMetrics
//================================================================//
class MOAIFontFaceMetrics {
public:

	float		mHeight;
	float		mAscent;
	
	//----------------------------------------------------------------//
				MOAIFontFaceMetrics		();
};

//================================================================//
// MOAIGlyphMetrics
//================================================================//
class MOAIGlyphMetrics {
public:

	float		mAdvanceX; // The distance the pen moves to the next glyph
	float		mBearingX; // The distance from the pen's x coordinate to the bounding box
	float		mBearingY; // The distance from the pen's y coordinate to the bounding box

	float		mWidth; // width of the glyph's bounding box (in pixels)
	float		mHeight; // height of the glyph's bounding box (in pixels)
	
	//----------------------------------------------------------------//
				MOAIGlyphMetrics		();
};

//================================================================//
// MOAIFontReader
//================================================================//
// this is a class to abstract getting glyph information from font
// files and rendering glyphs
class MOAIFontReader :
	public MOAILuaObject {
protected:

	enum {
		GLYPH_CODE_NULL = -1,
	};

	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_getFaceMetrics			( lua_State* L );
	static int		_getGlyphMetrics		( lua_State* L );
	static int		_getKernVec				( lua_State* L );
	static int		_hasKerning				( lua_State* L );
	static int		_open					( lua_State* L );
	static int		_renderGlyph			( lua_State* L );
	static int		_selectFace				( lua_State* L );
	static int		_selectGlyph			( lua_State* L );

public:

	enum {
		OK,
		NO_KERNING,
		FONT_ERROR,
	};

	//----------------------------------------------------------------//
	virtual int			CloseFontFile			();
	virtual int			GetFaceMetrics			( MOAIFontFaceMetrics& faceMetrics ) = 0;
	virtual int			GetGlyphMetrics			( MOAIGlyphMetrics& glyphMetrics ) = 0;
	virtual int			GetKernVec				( u32 c, MOAIKernVec& kernVec );
	virtual bool		HasKerning				();
						MOAIFontReader			();
						~MOAIFontReader			();
	virtual int			OpenFontFile			( cc8* filename );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	int					RenderGlyph				( MOAIImage& image, float x, float y );
	virtual int			RenderGlyph				( MOAIImage& image, float x, float y, const ZLColorBlendFunc& blendFunc ) = 0;
	virtual int			SelectFace				( float size ) = 0;
	virtual int			SelectGlyph				( u32 c ) = 0;
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
