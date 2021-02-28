// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFONTREADER_H
#define	MOAIFONTREADER_H

class MOAIImage;
class MOAIKernVec;

// we keep font face and glyph dimensions in pixels rather than UVs to make it easier to generate the
// screen rect for the glyph and also to keep the glyph's UV bounds from being invalidated
// if the texture page gets resized to make room for more glyphs

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
	
	GET ( float, Width, mWidth );
	GET ( float, Height, mHeight );
	GET_SET ( float, AdvanceX, mAdvanceX );
	
	//----------------------------------------------------------------//
	ZLRect		GetGlyphRect			( float x, float y, float xScale = 1.0f, float yScale = 1.0f ) const;
	ZLRect		GetLogicalRect			( float x, float y, float ascent, float descent, float xScale = 1.0f, float yScale = 1.0f ) const;
				MOAIGlyphMetrics		();
};

//================================================================//
// MOAIFontFaceMetrics
//================================================================//
class MOAIFontFaceMetrics {
public:

	float		mHeight;
	float		mAscent;
	
	GET_SET_CONST	( float, Ascent, mAscent );
	GET_CONST		( float, Descent, ( mHeight - mAscent ));
	
	//----------------------------------------------------------------//
				MOAIFontFaceMetrics		();
};

//================================================================//
// MOAIKernVec
//================================================================//
class MOAIKernVec :
	public ZLVec2D {
public:

	u32			mName;
};

//================================================================//
// MOAIFontReader
//================================================================//
// this is a class to abstract getting glyph information from font
// files and rendering glyphs
class MOAIFontReader :
	public virtual MOAILuaObject {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	static const u32 GLYPH_CODE_NULL = 0xffffffff;

	ZLColorBlendFunc	mBlendMode;

	//----------------------------------------------------------------//
	static int			_close					( lua_State* L );
	static int			_getFaceMetrics			( lua_State* L );
	static int			_getGlyphMetrics		( lua_State* L );
	static int			_getKernVec				( lua_State* L );
	static int			_hasKerning				( lua_State* L );
	static int			_open					( lua_State* L );
	static int			_renderGlyph			( lua_State* L );
	static int			_selectFace				( lua_State* L );
	static int			_selectGlyph			( lua_State* L );
	static int			_setBlendMode			( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

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
						MOAIFontReader			( ZLContext& context );
						~MOAIFontReader			();
	virtual int			OpenFontFile			( cc8* filename );
	virtual int			RenderGlyph				( MOAIImage& image, float x, float y ) = 0;
	virtual int			SelectFace				( float size ) = 0;
	virtual int			SelectGlyph				( u32 c ) = 0;
};

#endif
