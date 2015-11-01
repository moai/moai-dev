// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTLAYOUTRULES_H
#define	MOAITEXTLAYOUTRULES_H

class MOAIAnimCurve;
class MOAINode;
class MOAITextLayout;
class MOAITextStyle;
class MOAITextStyleCache;
class MOAITextStyleMap;
class MOAITextStyleSpan;

//================================================================//
// MOAITextLayoutRules
//================================================================//
class MOAITextLayoutRules {
protected:

	friend class MOAITextLayoutEngine;

	MOAINode*	mOwner; // TODO: this sucks. kill it.

	ZLRect		mFrame;
	
	bool		mLimitWidth;
	bool		mLimitHeight;
	
	u32			mHAlign;
	u32			mVAlign;
	bool		mYFlip;
	
	u32			mWordBreakRule;			// rule for breaking words across lines
	
	u32			mHLayoutSizingRule;		// controls sizing *and* alignment
	u32			mVLayoutSizingRule;		// controls sizing *and* alignment
	u32			mLineSizingRule;		// selects the edge to advance from
	
	float		mGlyphScale;
	float		mLineSpacing;
	
	float		mHLineSnap; // snap lines to this boundary
	float		mVLineSnap; // snap lines to this boundary
	
	ZLLeanArray < MOAIAnimCurve* > mCurves;

	//----------------------------------------------------------------//
	void		ReleaseCurve				( MOAIAnimCurve* curve  );
	void		RetainCurve					( MOAIAnimCurve* curve  );

public:

	enum {
		BASELINE_JUSTIFY,
		BOTTOM_JUSTIFY,
		LEFT_JUSTIFY,
		CENTER_JUSTIFY,
		RIGHT_JUSTIFY,
		TOP_JUSTIFY,
	};

	enum {
		WORD_BREAK_NONE,
		WORD_BREAK_CHAR,
	};
	
	enum {
		GLYPH_SIZE,
		LOGICAL_SIZE,
		MAXIMUM_SIZE,				// logical size plus padding
		VISIBLE_SIZE,				// glyph size plus padding
		VISIBLE_OR_LOGICAL_SIZE,	// greater of visible or logical size
	};

	GET_SET ( MOAINode*, Owner, mOwner )
	
	GET_SET_CONST ( ZLRect&, Frame, mFrame )
	
	GET_SET_CONST ( bool, LimitWidth, mLimitWidth )
	GET_SET_CONST ( bool, LimitHeight, mLimitHeight )
	
	GET_SET_CONST ( u32, HAlign, mHAlign )
	GET_SET_CONST ( u32, VAlign, mVAlign )
	GET_SET_CONST ( bool, YFlip, mYFlip )
	
	GET_SET_CONST ( u32, WordBreakRule, mWordBreakRule )
	GET_SET_CONST ( float, GlyphScale, mGlyphScale )
	GET_SET_CONST ( float, LineSpacing, mLineSpacing )

	GET_SET_CONST ( float, HLineSnap, mHLineSnap )
	GET_SET_CONST ( float, VLineSnap, mVLineSnap )

	//----------------------------------------------------------------//
	void				ClearCurves					();
	static ZLRect		GetGlyphRect				( const MOAIGlyph& glyph, float x, float y, float xScale, float yScale, u32 hRule, u32 vRule );
	ZLRect				GetGlyphLayoutRect			( const MOAIGlyph& glyph, float x, float y, float xScale, float yScale );
	ZLRect				GetGlyphSpacingRect			( const MOAIGlyph& glyph, float x, float y, float xScale, float yScale );
	void				Layout						( MOAITextLayout& layout, MOAITextStyleCache& styleCache, MOAITextStyleMap& styleMap, cc8* str, u32 idx, bool* more, u32* nextIdx, bool* overrun );
	void				Init						( const MOAITextLayoutRules& designer );
						MOAITextLayoutRules			();
						~MOAITextLayoutRules			();
	void				ReserveCurves				( u32 total );
	void				SetCurve					( u32 idx, MOAIAnimCurve* curve );
};

#endif
