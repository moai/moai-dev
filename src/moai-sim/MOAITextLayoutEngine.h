// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTLAYOUTENGINE_H
#define	MOAITEXTLAYOUTENGINE_H

#include <moai-sim/MOAITextShaper.h>

class MOAITextLayoutRules;
class MOAITextLayout;
class MOAITextStyle;
class MOAITextStyleCache;
class MOAITextStyleMap;
class MOAITextStyler;
class MOAITextStyleSpan;
class MOAITextStyleState;

//================================================================//
// MOAITextLayoutEngine
//================================================================//
// parser for producing a layout
class MOAITextLayoutEngine :
	public MOAITextShaperClient {
private:
	
	enum {
		RESTORE_POINT_CHAR,
		RESTORE_POINT_TOKEN,
		TOTAL_RESTORE_POINTS,
	};
	
	//================================================================//
	// RestorePoint
	//================================================================//
	class RestorePoint {
		public:
		
		u32						mCharIdx;
		u32						mSpriteIdx;
		ZLRect					mLineLayoutBounds;
		ZLRect					mLineSpacingBounds;
	};
	
	// a note about terminology:
	// *layout* bounds (as opposed to *logical*) bounds are calculated based on the
	// text designer's sizing/alignment rule. they may be distinct from the glyph
	// bounds, the visible bounds and the logical bounds.
	
	//----------------------------------------------------------------//
	// layout state
	
	MOAITextStyleSpan*		mStyleSpan;
	MOAITextStyleState*		mStyle;
	u32						mSpanIdx;
	
	u32						mCharIdx;
	
	cc8*					mStr;
	
	u32						mLineSpriteIdx;
	
	ZLRect					mLayoutBounds;
	ZLRect					mLineLayoutBounds;
	ZLRect					mLineSpacingBounds;
	float					mLineSpacingCursor;
	
	float					mEmptyLineAscent;
	float					mEmptyLineDescent;
	
	u32						mBaseLine;
	
	MOAITextStyledChar		mCurrentChar;
	MOAIGlyphSet*			mCurrentGlyphDeck;
	
	RestorePoint			mRestorePoints [ TOTAL_RESTORE_POINTS ];
	
	//----------------------------------------------------------------//
	// layout settings
	
	MOAITextLayoutRules*	mLayoutRules;
	MOAITextLayout*			mLayout;
	MOAITextStyleCache*		mStyleCache;
	MOAITextStyleMap*		mStyleMap;
	
	//----------------------------------------------------------------//
	void					Align						();
	void					BeginChar					();
	void					BeginToken					();
	void					BuildLayout					();
	void					CaptureRestorePoint			( u32 restorePointID );
	u32						GetCharIdx					();
	u32						GetLineSizeInSprites		();
	u32						GetSpriteIdx				();
	MOAITextStyledChar		NextChar					();
	u32						PushLine					();
	u32						PushSprite					( const MOAITextStyledChar& styledChar, float x, float y );
	void					Restore						( u32 restorePointID );
	float					Snap						( float f, float b );

public:

	GET ( u32, Index, mCharIdx )

	//----------------------------------------------------------------//
	void					BuildLayout					( MOAITextLayout& layout, MOAITextStyleCache& styleCache, MOAITextStyleMap& styleMap, MOAITextLayoutRules& designer, cc8* str, u32 idx );
							MOAITextLayoutEngine		();
	virtual					~MOAITextLayoutEngine		();
	bool					More						();
	bool					Overrun						();
};

#endif
