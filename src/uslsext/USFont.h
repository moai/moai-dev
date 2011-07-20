// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USFONT_H
#define	USFONT_H

#include <uslsext/USTexture.h>

class USAnimCurve;
class USGlyph;

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
	friend class USTextFrame;
	friend class USTextLayout;

	SET ( const USRect&, UVRect, mUVRect )
	
	SET ( float, AdvanceX, mAdvanceX )
	SET ( float, BearingX, mBearingX )
	
	GET_SET ( u32, Code, mCode )

	//----------------------------------------------------------------//
	void			Draw				( float points, float x, float y ) const;
	USKernVec		GetKerning			( u32 name ) const;
	USRect			GetRect				( float points, float x, float y ) const;
	void			ReserveKernTable	( u32 total );
	void			SerializeIn			( USLuaState& state );
	void			SerializeOut		( USLuaState& state );
	void			SetKernVec			( u32 id, const USKernVec& kernVec );
	void			SetScreenRect		( float width, float height, float yOff );
					USGlyph				();
					~USGlyph			();
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
	bool			IsWideChar			( u32 c );

public:

	friend class USTextFrame;

	enum {
		LEFT_JUSTIFY,
		RIGHT_JUSTIFY,
		CENTER_JUSTIFY,
	};

	GET_SET ( float, Scale, mScale )
	GET_SET ( float, LineSpacing, mLineSpacing )

	//----------------------------------------------------------------//
	USGlyph&		GetGlyphForChar		( u32 c );
	void			Init				( cc8* charCodes );
	void			SerializeIn			( USLuaState& state );
	void			SerializeOut		( USLuaState& state );
	void			SetGlyph			( const USGlyph& glyph );
	u32				Size				();
					USFont				();
	virtual			~USFont				();
};

#endif
