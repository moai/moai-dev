// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSTYLER_H
#define	MOAITEXTSTYLER_H

class MOAIAnimCurve;
class MOAIFont;
class MOAITextBox;
class MOAITextStyle;

//================================================================//
// MOAITextStyler
//================================================================//
class MOAITextStyler {
private:

	// max color hex digits
	static const u32 COLOR_MAX = 8;

	// style parser states
	enum {
		DONE,
		
		STYLE_START,
		STYLE_BODY,
		STYLE_FINISH,
		STYLE_ABORT,
		
		STYLE_NAME_START,
		STYLE_NAME_FINISH,
		
		STYLE_POP_START,
		STYLE_POP_FINISH,
		
		TOKEN_TEXT,
		
		COLOR_START,
		COLOR_BODY,
		COLOR_FINISH,
	};

	// color sizes
	enum {
		COLOR_GRAY_16		= 1,
		COLOR_GRAY_256		= 2,
		COLOR_RGB_16		= 3,
		COLOR_RGBA_16		= 4,
		COLOR_UNKNOWN		= 5,
		COLOR_RGB_256		= 6,
		COLOR_RGBA_256_16	= 7,
		COLOR_RGBA_256		= 8,
	};

	int						mIdx;
	int						mPrev;
	
	int						mTokenBase;
	int						mTokenTop;
	
	cc8*					mStr;
	MOAITextBox*			mTextBox;

	USLeanStack < MOAITextStyle*, 8 > mStyleStack;
	USLeanStack < MOAITextStyle*, 8 > mActiveStyles;

	MOAITextStyle* mCurrentStyle;

	//----------------------------------------------------------------//
	u32				AffirmStyle			( MOAITextStyle& style );
	void			FinishToken			();
	u32				GetChar				();
	u32				PackColor			( const u8* color, u32 colorSize );
	void			Parse				();
	bool			ParseStyle			();
	void			PopStyle			();
	void			PushStyle			( MOAITextStyle* styleState );
	void			UngetChar			();

public:

	//----------------------------------------------------------------//
	void			BuildStyleMap		( MOAITextBox& textBox );
					MOAITextStyler		();
					~MOAITextStyler		();
};

#endif
