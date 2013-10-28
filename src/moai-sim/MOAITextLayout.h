// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTLAYOUT_H
#define	MOAITEXTLAYOUT_H

#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAIProp.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleParser.h>

//================================================================//
// MOAITextSprite
//================================================================//
class MOAITextSprite {
private:

	friend class MOAITextLabel;
	friend class MOAITextDesignParser;
	friend class MOAITextLayout;
	
	MOAIGlyph*				mGlyph;
	MOAITextStyle*			mStyle;
	MOAITextureBase*		mTexture; // caching this here to avoid add'l virtual calls when drawing
	
	u32			mIdx; // index in original string
	float		mX;   // The pen position's x coordinate
	float		mY;   // The pen position's y coordinate
	float		mScale;
	u32			mRGBA;
	u32			mMask;
	
	enum {
		MASK_COLOR	= 0x01,
	};
};

//================================================================//
// MOAITextLine
//================================================================//
class MOAITextLine {
private:

	friend class MOAITextDesignParser;
	friend class MOAITextLabel;
	friend class MOAITextLayout;
	
	u32			mStart;		// index in sprite stack
	u32			mSize;		// number of sprites in line;
	ZLRect		mRect;		// tight bounds of glyphs in the line
	float		mHeight;	// total height of the line (irrespective of the glyphs)
	float		mAscent;	// offset to the text baseline

public:

};

//================================================================//
// MOAITextHighlight
//================================================================//
class MOAITextHighlight {

	friend class MOAITextLabel;
	friend class MOAITextLayout;

	u32		mBase;
	u32		mTop;
	u32		mColor;

	MOAITextHighlight* mPrev;
	MOAITextHighlight* mNext;

public:
};

//================================================================//
// MOAITextLayout
//================================================================//
class MOAITextLayout {
private:
	
	friend class MOAITextDesignParser;
	
	// this is the text page layout. these are the actual sprites and lines
	// that will be rendered for the current page.
	ZLLeanStack < MOAITextSprite, 64 >	mSprites;
	ZLLeanStack < MOAITextLine, 8 >		mLines;
	ZLRect								mBounds;
	
	float								mYOffset; // offset for Y flip
	
	// list of highlight spans
	MOAITextHighlight*					mHighlights;
	
	//----------------------------------------------------------------//
	void				CompactHighlights		();
	void				FindSpriteSpan			( u32 idx, u32 size, u32& spanIdx, u32& spanSize );
	void				PushLine				( u32 start, u32 size, const ZLRect& rect, float height, float ascent );
	void				PushSprite				( u32 idx, MOAIGlyph& glyph, MOAITextStyle& style, float x, float y, float scale );
	void				PushStyleSpan			( int base, int top, MOAITextStyle& style );
	
public:

	GET ( float, YOffset, mYOffset )

	//----------------------------------------------------------------//
	void				AddHighlight			( u32 base, u32 top, u32 color );
	void				ApplyHighlights			();
	void				ClearHighlight			( u32 base, u32 top );
	void				ClearHighlights			();
	u32					CountSprites			();
	void				Draw					( u32 reveal );
	void				DrawDebug				();
	bool				GetBounds				( ZLRect& rect );
	bool				GetBoundsForRange		( u32 idx, u32 size, ZLRect& rect );
						MOAITextLayout			();
						~MOAITextLayout			();
	void				RemoveHighlight			( MOAITextHighlight& highlight );
	void				Reset					();
	void				ResetHighlights			();
	void				SetHighlight			( u32 idx, u32 size );
	void				SetHighlight			( u32 idx, u32 size, u32 color );
};

#endif
