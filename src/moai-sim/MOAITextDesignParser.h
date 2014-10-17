// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTDESIGNPARSER_H
#define	MOAITEXTDESIGNPARSER_H

class MOAITextDesigner;
class MOAITextLayout;
class MOAITextStyle;
class MOAITextStyleCache;
class MOAITextStyleMap;
class MOAITextStyler;
class MOAITextStyleSpan;

//================================================================//
// MOAITextDesignParser
//================================================================//
class MOAITextDesignParser {
private:
	
	//----------------------------------------------------------------//
	// layout state
	
	MOAITextStyleSpan*	mStyleSpan;
	MOAITextStyle*		mStyle;
	u32					mSpanIdx;
	
	int					mIdx;
	int					mPrevIdx;
	
	cc8*				mStr;
	
	MOAIGlyphSet*		mDeck;
	float				mDeckScale;
	
	int					mLineIdx;
	u32					mLineSpriteID;
	u32					mLineSize;
	float				mLineHeight;
	float				mLineAscent;
	ZLRect				mLineRect;
	
	int					mTokenIdx;
	u32					mTokenSpriteID;
	u32					mTokenSize;
	float				mTokenHeight;
	float				mTokenAscent;
	ZLRect				mTokenRect;
	
	float				mLayoutWidth;
	float				mLayoutHeightMin;
	float				mLayoutHeightMax;
	
	float				mPenX;
	ZLVec2D				mOffset;
	MOAIGlyph*			mPrevGlyph;
	bool				mMore;
	
	u32					mBaseLine;
	
	//----------------------------------------------------------------//
	// layout settings
	
	MOAITextDesigner*		mDesigner;
	MOAITextLayout*			mLayout;
	MOAITextStyleCache*		mStyleCache;
	MOAITextStyleMap*		mStyleMap;
	
	//----------------------------------------------------------------//
	void				AcceptLine					();
	void				AcceptToken					();
	void				Align						();
	void				BuildLayout					();
	float				GetLayoutHeight				();
	u32					NextChar					();
	float				Snap						( float f, float b );

public:

	GET ( u32, Index, mIdx )

	//----------------------------------------------------------------//
	void				BuildLayout					( MOAITextLayout& layout, MOAITextStyleCache& styleCache, MOAITextStyleMap& styleMap, MOAITextDesigner& designer, cc8* str, u32 idx, ZLVec2D& offset );
						MOAITextDesignParser		();
	virtual				~MOAITextDesignParser		();
	bool				More						();
};

#endif
