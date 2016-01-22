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
// MOAILayoutEngineState
//================================================================//
class MOAILayoutEngineState {
protected:

	friend class MOAITextLayoutEngine;
	
	MOAITextStyleSpan*		mStyleSpan;
	u32						mSpanIdx;
	
	u32						mCharIdx;
	u32						mSpriteIdx;
	ZLRect					mLineLayoutBounds;
	ZLRect					mLineSpacingBounds;
	
	//----------------------------------------------------------------//
			MOAILayoutEngineState		();
			~MOAILayoutEngineState		();
};

//================================================================//
// MOAITextLayoutEngine
//================================================================//
// parser for producing a layout
class MOAITextLayoutEngine :
	public MOAITextShaperClient,
	public MOAILayoutEngineState {
private:
	
	enum {
		RESTORE_POINT_CHAR,
		RESTORE_POINT_TOKEN,
		RESTORE_POINT_LINE,
		TOTAL_RESTORE_POINTS,
	};
	
	// a note about terminology:
	// *layout* bounds (as opposed to *logical*) bounds are calculated based on the
	// text designer's sizing/alignment rule. they may be distinct from the glyph
	// bounds, the visible bounds and the logical bounds.
	
	//----------------------------------------------------------------//
	// layout state
	
	cc8*					mStr;

	ZLRect					mLayoutBounds;
	
	float					mLineSpacingCursor;
	float					mEmptyLineAscent;
	float					mEmptyLineDescent;
	
	u32						mBaseLine;
	
	MOAITextStyledChar		mCurrentChar;
	MOAIGlyphSet*			mCurrentGlyphDeck;
	
	MOAILayoutEngineState	mRestorePoints [ TOTAL_RESTORE_POINTS ];
	
	bool					mResetStyle;
	bool					mOverrun;
	
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
	u32						GetLineSizeInSprites		();
	u32						GetLineSpriteIdx			();
	u32						GetSpriteIndex				();
	MOAITextStyledChar		NextChar					();
	u32						PushLine					();
	u32						PushSprite					( const MOAITextStyledChar& styledChar, float x, float y );
	void					Restore						( u32 restorePointID );
	float					Snap						( float f, float b );

public:

	//----------------------------------------------------------------//
	void					BuildLayout					( MOAITextLayout& layout, MOAITextStyleCache& styleCache, MOAITextStyleMap& styleMap, MOAITextLayoutRules& layoutRules, cc8* str, u32 idx );
	u32						GetCharIndex				();
							MOAITextLayoutEngine		();
	virtual					~MOAITextLayoutEngine		();
	bool					More						();
	bool					Overrun						();
};

#endif
