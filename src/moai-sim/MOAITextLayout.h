// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTLAYOUT_H
#define	MOAITEXTLAYOUT_H

#include <moai-sim/MOAITextShaper.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleParser.h>

class MOAIShader;
class MOAITextStyledChar;

//================================================================//
// MOAITextSprite
//================================================================//
class MOAITextSprite {
private:

	friend class MOAITextLabel;
	friend class MOAITextLayoutEngine;
	friend class MOAITextLayout;
	
	MOAIGlyph*				mGlyph;
	MOAITextStyleState*		mStyle;
	MOAITexture*		mTexture; // caching this here to avoid add'l virtual calls when drawing
	MOAIShader*		mShader;
	
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

	friend class MOAITextLayoutEngine;
	friend class MOAITextLabel;
	friend class MOAITextLayout;
	
	ZLIndex		mStart;				// index in sprite stack
	ZLSize		mSize;				// number of sprites in line;
	
	ZLVec2D		mOrigin;			// offset to line 'hotspot' - origin of drawing
	ZLRect		mGlyphBounds;		// tight fitting glyph bounds
	ZLRect		mLayoutBounds;		// bounds used for layout and alignment of line

public:

	//----------------------------------------------------------------//
	float		GetAscent			();
	float		GetDescent			();
	void		Offset				( float xOff, float yOff );
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
// this is a buffer that holds glyphs once they've been laid out
class MOAITextLayout {
private:
	
	friend class MOAITextLayoutEngine;
	friend class MOAITextLabel;
	
	// this is the text page layout. these are the actual sprites and lines
	// that will be rendered for the current page.
	ZLLeanStack < MOAITextSprite, 64 >	mSprites;
	ZLLeanStack < MOAITextLine, 8 >		mLines;
	
	ZLRect								mGlyphFrame;
	ZLRect								mLayoutFrame; // bounds used for sizing and alignment
	
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
	void				FindSpriteSpan			( u32 idx, u32 size, ZLIndex& spanIdx, ZLSize& spanSize );
	void				PushLine				( ZLIndex start, ZLSize size, const ZLVec2D& origin, const ZLRect& layoutBounds );
	void				PushSprite				( const MOAITextStyledChar& styledChar, float x, float y );
	
public:

	GET ( const ZLRect&, LayoutFrame, mLayoutFrame )

	//----------------------------------------------------------------//
	void				AddHighlight			( u32 base, u32 top, u32 color );
	void				ApplyHighlights			();
	void				ClearHighlight			( u32 base, u32 top );
	void				ClearHighlights			();
	size_t				CountSprites			();
	void				Draw					( MOAIGfxMgr& gfxMgr, u32 reveal );
	void				DrawDebug				( MOAIDebugLinesMgr& debugLines, MOAIDraw& draw );
	bool				GetFrame				( ZLRect& frame );
	bool				GetFrameForRange		( u32 idx, u32 size, ZLRect& frame );
						MOAITextLayout			();
						~MOAITextLayout			();
	void				RemoveHighlight			( MOAITextHighlight& highlight );
	void				Reset					();
	void				ResetHighlights			();
	void				SetHighlight			( u32 idx, u32 size );
	void				SetHighlight			( u32 idx, u32 size, u32 color );
};

#endif
