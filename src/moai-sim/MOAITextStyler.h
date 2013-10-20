// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSTYLER_H
#define	MOAITEXTSTYLER_H

class MOAINode;
class MOAITextStyle;
class MOAITextStyleParser;

//================================================================//
// MOAITextStyleRef
//================================================================//
class MOAITextStyleRef {
private:

	friend class MOAITextBox;
	friend class MOAITextStyler;

	MOAITextStyle*			mStyle;
	MOAITextStyleState		mState;

public:

	//----------------------------------------------------------------//
				MOAITextStyleRef		();
				~MOAITextStyleRef		();
	bool		NeedsLayout				() const;
	void		UpdateState				();
};

//================================================================//
// MOAITextStyleSpan
//================================================================//
class MOAITextStyleSpan {
public:

	int					mBase;		// base index of first utf-8 character in span
	int					mTop;		// size of span
	MOAITextStyle*		mStyle;		// style for span
};

//================================================================//
// MOAITextStyler
//================================================================//
class MOAITextStyler {
private:

	friend class MOAITextDesigner;
	friend class MOAITextStyleParser;

	// set only if we care about LuaRelease, LuaRetain and setting dep links to styles
	MOAINode* mOwner;

	// the text we are styling lives here
	

	// style set - these are the styles the texbox knows about
	// only need to get these during text styling, so using an STLMap for now...
	typedef STLMap < STLString, MOAITextStyleRef >::iterator StyleSetIt;
	STLMap < STLString, MOAITextStyleRef > mStyleSet;
	
	// anonymous styles - these are created on the fly as text is being styled
	ZLLeanStack < MOAITextStyleRef, 8 > mAnonymousStyles;
	
	// this is the style map. it is produced by analyzing the text and creating a
	// 'style span' for each styled token. this is the preprocessing step to
	// actually layout out a page of text. text is laid out based on the style spans.
	ZLLeanStack < MOAITextStyleSpan, 64 > mStyleMap; // each span represents a stretch of 'styled' text
	
	//----------------------------------------------------------------//
	MOAITextStyle*		AddAnonymousStyle		( MOAITextStyle* source );
	void				PushStyleSpan			( int base, int top, MOAITextStyle& style );
	void				ReleaseStyle			( MOAITextStyle* style );
	void				RetainStyle				( MOAITextStyle* style );

public:
	
	//----------------------------------------------------------------//
	void				BuildStyleMap			( cc8* str );
	bool				CheckStylesChanged		();
	u32					CountSpans				();
	MOAITextStyle*		GetStyle				();
	MOAITextStyle*		GetStyle				( cc8* styleName );
						MOAITextStyler			();
						~MOAITextStyler			();
	void				RefreshStyleGlyphs		( cc8* str );
	void				ResetStyleSet			();
	void				SetStyle				( MOAITextStyle* style );
	void				SetStyle				( cc8* styleName, MOAITextStyle* style );
	void				ResetStyleMap			();
};

#endif
