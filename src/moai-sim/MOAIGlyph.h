// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPH_H
#define	MOAIGLYPH_H

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
	static const u32 NULL_CODE_ID			= 0xffffffff;
	static const u32 NULL_PAGE_ID			= 0xffffffff;
	
	u32			mCode;   // The character code of the glyph
	u32			mPageID; // ID of texture page in glyph cache
	
	float		mWidth; // width of the bounding box in pixels
	float		mHeight; // height of the bounding box in pixels
	float		mAdvanceX; // The distance the pen moves to the next glyph
	float		mBearingX; // The distance from the pen's x coordinate to the bounding box
	float		mBearingY; // The distance from the pen's y coordinate to the bounding box
	
	u32			mSrcX; // corresponds to glyph location on page
	u32			mSrcY; // corresponds to glyph location on page
	
	ZLLeanArray < MOAIKernVec > mKernTable;
	
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
	
	GET_SET ( u32, Code, mCode );
	GET_SET ( u32, PageID, mPageID );
	GET_SET ( float, AdvanceX, mAdvanceX );

	//----------------------------------------------------------------//
	void			Draw				( MOAITextureBase& texture, float x, float y, float scale ) const;
	MOAIKernVec		GetKerning			( u32 name ) const;
	ZLRect			GetRect				( float x, float y, float scale = 1) const;
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
