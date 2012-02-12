// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPH_H
#define	MOAIGLYPH_H

#include <moaicore/MOAILua.h>

class MOAIGlyphPage;

//================================================================//
// MOAIKernVec
//================================================================//
class MOAIKernVec :
	public USVec2D {
public:

	u32			mName;
};

//================================================================//
// MOAIGlyph
//================================================================//
class MOAIGlyph {
private:

	u32			mCode;
	u32			mContents;

	u32			mSrcX; // corresponds to glyph location on page
	u32			mSrcY; // corresponds to glyph location on page

	u32			mWidth; // width in pixels
	u32			mHeight; // height in pixels
	
	float		mYOff;
	
	float		mAdvanceX;
	float		mBearingX;

	USLeanArray < MOAIKernVec > mKernTable;
	
	MOAIGlyphPage*	mPage; // page containing glyph texture (if any)
	MOAIGlyph*		mNext; // for use in glyph processing list
	
public:

	friend class MOAIFont;
	friend class MOAIGlyphDeck;
	friend class MOAIGlyphPage;
	friend class MOAITextStyler;
	friend class MOAITextLayout;
	
	SET ( float, AdvanceX, mAdvanceX )
	SET ( float, BearingX, mBearingX )
	
	GET_SET ( u32, Code, mCode )

	enum {
		NONE,
		METRICS_ONLY,
		MATRICS_AND_BITMAP,
	};

	//----------------------------------------------------------------//
	void			Draw				( float points, float x, float y ) const;
	MOAIKernVec		GetKerning			( u32 name ) const;
	USRect			GetRect				( float points, float x, float y ) const;
					MOAIGlyph			();
					~MOAIGlyph			();
	void			ReserveKernTable	( u32 total );
	void			SerializeIn			( MOAILuaState& state );
	void			SerializeOut		( MOAILuaState& state );
	void			SetKernVec			( u32 id, const MOAIKernVec& kernVec );
	void			SetScreenRect		( float width, float height, float yOff );
};

#endif
