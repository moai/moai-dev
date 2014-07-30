// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPH_H
#define	MOAIGLYPH_H

#include <moai-sim/MOAIFontReader.h>

class MOAIDynamicGlyphCachePage;
class MOAITextureBase;

//================================================================//
// MOAIGlyph
//================================================================//
class MOAIGlyph :
	public MOAIGlyphMetrics {
private:
	
	static const u32 MAX_KERN_TABLE_SIZE	= 512;
	static const u32 NULL_CODE_ID			= 0xffffffff;
	static const u32 NULL_PAGE_ID			= 0xffffffff;
	
	u32			mCode;   // The character code of the glyph
	u32			mPageID; // ID of texture page in glyph cache
	
	u32			mSrcX; // corresponds to glyph location on page (in pixels)
	u32			mSrcY; // corresponds to glyph location on page (in pixels)
	
	ZLLeanArray < MOAIKernVec > mKernTable;
	
	MOAIGlyph*	mNext; // for use in glyph processing list
	
public:

	// TODO: this is a lot of friends; good idea to clean this up
	friend class MOAIBitmapFontReader;
	friend class MOAIFont;
	friend class MOAIFreeTypeFontReader;
	friend class MOAIGlyphSet;
	friend class MOAIGlyphCache;
	friend class MOAIDynamicGlyphCachePage;
	friend class MOAITextLabel;
	friend class MOAITextDesignParser;
	friend class MOAITextLayout;
	friend class MOAITextStyleParser;
	
	GET ( u32, SrcX, mSrcX );
	GET ( u32, SrcY, mSrcY );
	GET ( float, Width, mWidth );
	GET ( float, Height, mHeight );
	
	GET_SET ( u32, Code, mCode );
	GET_SET ( u32, PageID, mPageID );
	GET_SET ( float, AdvanceX, mAdvanceX );

	//----------------------------------------------------------------//
	void			Draw				( MOAITextureBase& texture, float x, float y, float xScale, float yScale, const ZLRect& padding ) const;
	MOAIKernVec		GetKerning			( u32 name ) const;
	ZLRect			GetRect				( float x, float y, float xScale = 1.0f, float yScale = 1.0f ) const;
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
