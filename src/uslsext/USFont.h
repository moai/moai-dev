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

	const USGlyph*	mLastGlyph;

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

	u32			mCode;
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
	
	GET_SET ( u32, Code, mCode )

	//----------------------------------------------------------------//
	void			Draw				( float points, float x, float y ) const;
	USKernVec		GetKerning			( u32 name ) const;
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

	static const u32 WIDE_ID_BIT	= 0x80000000;
	static const u32 WIDE_ID_MASK	= 0x7fffffff;
	static const u32 INVALID_ID		= 0xffffffff;

	USLeanArray < USGlyph >		mByteGlyphs;
	USLeanArray < u8 >			mByteGlyphMap;
	u8							mByteGlyphMapBase;

	USLeanArray < USGlyph >		mWideGlyphs;
	USLeanArray < u32 >			mWideGlyphMap;

	float mScale;
	float mLineSpacing;
	
	USGlyph mDummy;

	//----------------------------------------------------------------//
	void			DrawGlyph			( u32 c, float points, float x, float y );
	u32				GetIDForChar		( u32 c );
	USGlyph&		GetGlyphForID		( u32 id );
	USTextLine		GetLine				( cc8* str, float points, float width );
	USTextToken		GetToken		 	( cc8* str, float points, USTextToken* prevToken );
	bool			IsWideChar			( u32 c );
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
	const USGlyph&	GetGlyphForChar		( u32 c );
	void			Init				( cc8* charCodes );
	float			Justify				( float x, float width, float lineWidth, u32 justify );
	u32				Layout				( USGlyphBuffer& glyphBuffer, cc8* str, float points, USRect frame, u32 justify, USTextStyler& styler, USAnimCurve** curves, u32 totalCurves );
	void			SetGlyph			( const USGlyph& glyph );
	u32				Size				();
					USFont				();
	virtual			~USFont				();
};

#endif
