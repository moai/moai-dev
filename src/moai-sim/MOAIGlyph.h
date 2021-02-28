// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGLYPH_H
#define	MOAIGLYPH_H

#include <moai-sim/MOAIFontReader.h>

class MOAIDynamicGlyphCachePage;
class MOAIGlyphSet;
class MOAITexture;

//================================================================//
// MOAIGlyph
//================================================================//
class MOAIGlyph :
	public MOAIGlyphMetrics {
private:
	
	static const u32 MAX_KERN_TABLE_SIZE	= 512;
	
	u32			mCode;   // The character code of the glyph
	ZLIndex		mPageID; // ID of texture page in glyph cache
	
	u32			mSrcX; // corresponds to glyph location on page (in pixels)
	u32			mSrcY; // corresponds to glyph location on page (in pixels)
	
	ZLLeanArray < MOAIKernVec > mKernTable;
	
	MOAIGlyph*		mNext; // for use in glyph processing list
	MOAIGlyphSet*	mDeck;
	
public:

	static const u32 NULL_CODE_ID			= 0xffffffff;

	// TODO: this is a lot of friends; good idea to clean this up
	friend class MOAIBitmapFontReader;
	friend class MOAIFont;
	friend class MOAIFreeTypeFontReader;
	friend class MOAIGlyphSet;
	friend class MOAIGlyphCache;
	friend class MOAIDynamicGlyphCachePage;
	friend class MOAITextLabel;
	friend class MOAITextLayoutEngine;
	friend class MOAITextLayout;
	friend class MOAITextStyleParser;
	
	GET ( u32, SrcX, mSrcX )
	GET ( u32, SrcY, mSrcY )
	GET_SET ( u32, Code, mCode )
	GET_SET ( ZLIndex, PageID, mPageID )
	GET_CONST ( MOAIGlyphSet&, Deck, *mDeck )
	
	//----------------------------------------------------------------//
	void			Draw					( MOAIGfxMgr& gfxMgr, MOAITexture& texture, float x, float y, float xScale, float yScale, const ZLRect& padding ) const;
	ZLRect			GetGlyphLogicalRect		( float x, float y, float xScale = 1.0f, float yScale = 1.0f ) const;
	MOAIKernVec		GetKerning				( u32 name ) const;
					MOAIGlyph				();
					~MOAIGlyph				();
	void			ReserveKernTable		( ZLSize total );
	void			SerializeIn				( MOAILuaState& state );
	void			SerializeOut			( MOAILuaState& state );
	void			SetKernVec				( ZLIndex id, const MOAIKernVec& kernVec );
	//void			SetScreenRect			( float width, float height, float yOff );
	void			SetSourceLoc			( u32 srcX, u32 srcY );
};

#endif
