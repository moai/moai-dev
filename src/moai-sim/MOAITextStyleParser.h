// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTSTYLEPARSER_H
#define	MOAITEXTSTYLEPARSER_H

class MOAIAnimCurve;
class MOAIFont;
class MOAITextStyle;
class MOAITextStyleCache;
class MOAITextStyleMap;

//================================================================//
// MOAITextStyleParser
//================================================================//
// parser state for producing a style map
class MOAITextStyleParser {
private:

	friend class MOAITextStyleCache;
	friend class MOAITextStyleMap;

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
		
		PEN_START,
		PEN_FINISH,
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
	MOAITextStyleCache*		mStyleCache;
	MOAITextStyleMap*		mStyleMap;

	ZLLeanStack < MOAITextStyleState*, 8 > mStyleStack;
	ZLLeanStack < MOAITextStyleState*, 8 > mActiveStyles;

	MOAITextStyleState*	mDefaultStyle;
	MOAITextStyleState*	mCurrentStyle;

	//----------------------------------------------------------------//
	void			BuildStyleMap				( MOAITextStyleMap& styleMap, MOAITextStyleCache& styleCache, cc8* str );
	void			FinishToken					();
	u32				GetChar						();
	u32				PackColor					( const u8* color, u32 colorSize );
	void			Parse						();
	bool			ParseStyle					();
	void			PopStyle					();
	void			PushStyle					( MOAITextStyleState* styleState );
	void			UngetChar					();

public:

	//----------------------------------------------------------------//
					MOAITextStyleParser			();
					~MOAITextStyleParser		();
};

#endif
