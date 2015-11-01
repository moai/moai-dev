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
	public MOAITextStyledCharStream,
	public MOAITextLineLayout {
private:
	
	//----------------------------------------------------------------//
	// layout state
	
	MOAITextStyleSpan*		mStyleSpan;
	MOAITextStyleState*		mStyle;
	u32						mSpanIdx;
	
	int						mCharIdx;
	
	cc8*					mStr;
	
	int						mLineCharIdx;
	u32						mLineSpriteIdx;
	
	u32						mLineSizeInSprites;
	
	// *layout* bounds (as opposed to *logical*) bounds are calculated based on the
	// text designer's sizing/alignment rule. they may be distinct from the glyph
	// bounds, the visible bounds and the logical bounds.
	
	ZLRect					mLayoutBounds;
	ZLRect					mLineLayoutBounds;
	
	ZLRect					mLineSpacingBounds;
	ZLRect					mPrevLineSpacingBounds;
	
	u32						mBaseLine;
	
	MOAITextStyledChar		mCurrentChar;
	
	//----------------------------------------------------------------//
	// layout settings
	
	MOAITextLayoutRules*		mDesigner;
	MOAITextLayout*			mLayout;
	MOAITextStyleCache*		mStyleCache;
	MOAITextStyleMap*		mStyleMap;
	
	//----------------------------------------------------------------//
	u32						PushLine					();
	void					Align						();
	void					BuildLayout					();
	u32						GetCharIdx					();
	u32						GetSpriteIdx				();
	MOAITextStyledChar		NextChar					();
	u32						PushSprite					( const MOAITextStyledChar& styledChar, float x, float y );
	void					SeekChar					( u32 charIdx );
	void					SeekSprite					( u32 spriteIdx );
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
