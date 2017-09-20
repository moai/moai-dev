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

	MOAINode*	mOwner; // TODO: just put here to make release/retain curve work. which sucks. kill it.

	ZLRect		mFrame;
	
	bool		mLimitWidth;
	bool		mLimitHeight;
	
	u32			mHAlign;
	u32			mVAlign;
	bool		mYFlip;
	
	u32			mFirstOverrunRule;		// cannot be OVERRUN_MOVE_WORD
	u32			mOverrunRule;
	
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
		OVERRUN_MOVE_WORD, // not valid on first overflow
		OVERRUN_SPLIT_WORD,
		OVERRUN_TRUNCATE_WORD,
		OVERRUN_ABORT_LAYOUT, // reverts word
		
		OVERRUN_NONE, // for use by the shaper only
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
	
	GET_SET_CONST ( u32, FirstOverrunRule, mFirstOverrunRule )
	GET_SET_CONST ( u32, OverrunRule, mOverrunRule )
	GET_SET_CONST ( float, GlyphScale, mGlyphScale )
	GET_SET_CONST ( float, LineSpacing, mLineSpacing )

	GET_SET_CONST ( float, HLineSnap, mHLineSnap )
	GET_SET_CONST ( float, VLineSnap, mVLineSnap )

	GET_SET_CONST ( u32, HLayoutSizingRule, mHLayoutSizingRule )
	GET_SET_CONST ( u32, VLayoutSizingRule, mVLayoutSizingRule )
	GET_SET_CONST ( u32, LineSizingRule, mLineSizingRule )

	//----------------------------------------------------------------//
	void				ClearCurves					();
	static ZLRect		GetGlyphRect				( const MOAIGlyph& glyph, float x, float y, float xScale, float yScale, u32 hRule, u32 vRule );
	ZLRect				GetGlyphLayoutRect			( const MOAIGlyph& glyph, float x, float y, float xScale, float yScale );
	ZLRect				GetGlyphSpacingRect			( const MOAIGlyph& glyph, float x, float y, float xScale, float yScale );
	void				Init						( const MOAITextLayoutRules& designer );
	void				Layout						( MOAITextLayout& layout, MOAITextStyleCache& styleCache, MOAITextStyleMap& styleMap, cc8* str, u32 idx, bool* more, u32* nextIdx, bool* overrun );
						MOAITextLayoutRules			();
						~MOAITextLayoutRules		();
	void				ReserveCurves				( u32 total );
	void				SetCurve					( u32 idx, MOAIAnimCurve* curve );
};

#endif
