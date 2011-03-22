// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	FONT_H
#define	FONT_H

#include <uslsext/USTexture.h>

class USAnimCurve;
class USGlyph;

//================================================================//
// USTextStyler
//================================================================//
class USTextStyler {
private:

	u32 mRGBA;

public:

	friend class USFontStringParser;
	friend class USFont;

	//----------------------------------------------------------------//
	void	Reset			();
};

//================================================================//
// USTextToken
//================================================================//
class USTextToken {
public:
	
	enum {
		kEOF,
		kNewline,
		kWhitespace,
		kWord,
	};

	USGlyph*	mLastGlyph;

	u32			mLength;
	u32			mNonWhitespace;
	u32			mType;
		
	float		mAdvance;
	float		mWidth;
	float		mKern;
};

//================================================================//
// USTextLine
//================================================================//
class USTextLine {
public:

	bool	mIsFinal;		// last string parsed from input
	
	u32		mLength;		// total characters in line (including whitespace and markup)
	u32		mVisibleLength;	// total length to draw (including whitespace and markup)
	
	u32		mNonWhitespace;	// total renderable characters in visible length (no whitespace or markup)
	
	float	mAdvance;
	float	mWidth;
};

//================================================================//
// USKernVec
//================================================================//
class USKernVec :
	public USVec2D {
public:

	u32			mName;
};

//================================================================//
// USGlyph
//================================================================//
class USGlyph {
private:

	u32			mName;

	USRect		mUVRect;

	float		mWidth;
	float		mHeight;
	float		mYOff;
	
	float		mAdvanceX;
	float		mBearingX;

	USLeanArray < USKernVec > mKernTable;
	
public:

	friend class USFont;

	SET ( const USRect&, UVRect, mUVRect )
	
	SET ( float, AdvanceX, mAdvanceX )
	SET ( float, BearingX, mBearingX )
	
	GET_SET ( u32, Name, mName )

	//----------------------------------------------------------------//
	void			Draw				( float points, float x, float y );
	USKernVec		GetKerning			( u32 name );
	void			ReserveKernTable	( u32 total );
	void			SetKernVec			( u32 id, const USKernVec& kernVec );
	void			SetScreenRect		( float width, float height, float yOff );
					USGlyph				();
					~USGlyph			();
};

//================================================================//
// USGlyphSprite
//================================================================//
class USGlyphSprite {
private:

	friend class USGlyphBuffer;
	friend class USFont;
	
	float		mX;
	float		mY;
	u32			mRGBA;
	u32			mGlyphID;
	float		mPoints;
};

//================================================================//
// USGlyphBuffer
//================================================================//
class USGlyphBuffer :
	public USLeanStack < USGlyphSprite, 64 > {
public:

	//----------------------------------------------------------------//
	void		PushGlyph		( u32 glyphID, float x, float y, float points, u32 rgba );
};

//================================================================//
// USFont
//================================================================//
class USFont {
private:

	USLeanArray < USGlyph > mGlyphs;
	u8 mGlyphMap [ 256 ];

	float mScale;
	float mLineSpacing;

	//----------------------------------------------------------------//
	void			DrawGlyph			( u32 c, float points, float x, float y );
	void			EndDrawing			();
	USTextLine		GetLine				( cc8* str, float points, float width );
	USTextToken		GetToken			( cc8* str, float points, USTextToken* prevToken );
	void			LayoutLine			( USGlyphBuffer& glyphBuffer, cc8* str, float points, u32 size, float x, float y, USTextStyler& styler, USAnimCurve* curve, u32 width, u32 xOff );

public:

	enum {
		LEFT_JUSTIFY,
		RIGHT_JUSTIFY,
		CENTER_JUSTIFY,
	};

	GET_SET ( float, Scale, mScale )
	GET_SET ( float, LineSpacing, mLineSpacing )

	//----------------------------------------------------------------//
	void			Draw				( const USGlyphBuffer& layout, u32 reveal );
	USGlyph&		GetGlyph			( u32 id );
	USGlyph&		GetGlyphForChar		( u32 c );
	void			Init				( u32 totalGlyphs );
	float			Justify				( float x, float width, float lineWidth, u32 justify );
	u32				Layout				( USGlyphBuffer& glyphBuffer, cc8* str, float points, USRect frame, u32 justify, USTextStyler& styler, USAnimCurve** curves, u32 totalCurves );
	void			SetCharGlyphID		( u32 c, u32 id );
	void			SetGlyph			( u32 id, const USGlyph& glyph );
	void			SetGlyphMap			( cc8* mapString );
					USFont				();
					~USFont				();
};

#endif
