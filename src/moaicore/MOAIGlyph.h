// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPH_H
#define	MOAIGLYPH_H

#include <moaicore/MOAILua.h>

class MOAIGlyphCachePage;
class MOAITextureBase;

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
	
	static const u32 MAX_KERN_TABLE_SIZE	= 512;
	static const u32 NULL_PAGE_ID			= 0xffffffff;
	
	u32			mCode;
	u32			mPageID; // ID of texture page in glyph cache
	
	float		mWidth; // width in pixels
	float		mHeight; // height in pixels
	float		mAdvanceX;
	float		mBearingX;
	float		mBearingY;
	
	u32			mSrcX; // corresponds to glyph location on page
	u32			mSrcY; // corresponds to glyph location on page
	
	USLeanArray < MOAIKernVec > mKernTable;
	
	MOAIGlyph*	mNext; // for use in glyph processing list
	
public:

	// TODO: this is a lot of friends; good idea to clean this up
	friend class MOAIBitmapFontReader;
	friend class MOAIFont;
	friend class MOAIFreeTypeFontReader;
	friend class MOAIGlyphSet;
	friend class MOAIGlyphCacheBase;
	friend class MOAIGlyphCachePage;
	friend class MOAITextBox;
	friend class MOAITextDesigner;
	friend class MOAITextStyler;

	GET_SET ( u32, PageID, mPageID );

	//----------------------------------------------------------------//
	void			Draw				( MOAITextureBase& texture, float x, float y, float scale ) const;
	MOAIKernVec		GetKerning			( u32 name ) const;
	USRect			GetRect				( float x, float y ) const;
					MOAIGlyph			();
					~MOAIGlyph			();
	void			ReserveKernTable	( u32 total );
	void			SerializeIn			( MOAILuaState& state );
	void			SerializeOut		( MOAILuaState& state );
	void			SetKernVec			( u32 id, const MOAIKernVec& kernVec );
	void			SetScreenRect		( float width, float height, float yOff );
	void			SetSourceLoc		( u32 srcX, u32 srcY );
};

#endif
