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
	
	u32			mIdx;		// index in original string
	ZLVec2D		mPen;		// The pen x and y coordinates
	ZLVec2D		mScale;
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
	friend class MOAITextLabel;
	
	// this is the text page layout. these are the actual sprites and lines
	// that will be rendered for the current page.
	ZLLeanStack < MOAITextSprite, 64 >	mSprites;
	ZLLeanStack < MOAITextLine, 8 >		mLines;
	ZLRect								mBounds;
	
	// calculated during alignment
	// - the text is laid out in model space with the origin at the *center* of the text frame
	// - centering the text makes it easy to flip the textbox by prepending a rotation or scale
	// - before the main transform, apply the offset to get the correct text position 
	float								mXOffset;
	float								mYOffset;
	
	// list of highlight spans
	MOAITextHighlight*					mHighlights;
	
	//----------------------------------------------------------------//
	void				CompactHighlights		();
	void				FindSpriteSpan			( u32 idx, u32 size, u32& spanIdx, u32& spanSize );
	void				PushLine				( u32 start, u32 size, const ZLRect& rect, float height, float ascent );
	void				PushSprite				( u32 idx, MOAIGlyph& glyph, MOAITextStyle& style, float x, float y, float xScale, float yScale );
	void				PushStyleSpan			( int base, int top, MOAITextStyle& style );
	
public:

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
