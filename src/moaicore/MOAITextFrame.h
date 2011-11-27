// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTFRAME_H
#define	MOAITEXTFRAME_H

class MOAIFont;
class MOAITextLayout;

//================================================================//
// MOAITextCursor
//================================================================//
class MOAITextCursor {
private:

	friend class MOAITextFrame;

	u32		mIndex;
	u32		mRGBA;

public:

	GET ( u32, Index, mIndex )
	GET ( u32, RGBA, mRGBA )
	
	//----------------------------------------------------------------//
	void	Reset	();
};

//================================================================//
// MOAITextFrame
//================================================================//
class MOAITextFrame {
private:

	// max color hex digits
	static const u32 COLOR_MAX = 8;

	// style parser states
	enum {
		META_START,
		META_FINISH,
		
		STYLE_START,
		STYLE_ABORT,
		
		TEXT_START,
		TEXT_FINISH_TOKEN,
		TEXT_FINISH_LINE,
		TEXT_FLUSH_TOKEN,
		
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

	int					mIdx;
	u32					mRGBA;
	MOAITextCursor*		mCursor;
	
	u32					mLineBottom;
	u32					mLineTop;
	
	u32					mLineCount;
	u32					mTotalLines;
	
	float				mLineXMax;
	float				mTokenXMin;
	float				mTokenXMax;
	
	USVec2D				mPen;
	cc8*				mStr;
	MOAIFont*			mFont;
	USRect				mFrame;
	float				mPoints;
	float				mLineSpacing;
	u32					mJustify;
	MOAITextLayout*		mLayout;
	const MOAIGlyph*	mGlyph;
	
	MOAIAnimCurve**		mCurves;
	u32					mTotalCurves;

	//----------------------------------------------------------------//
	u32				DecodeChar			();
	void			Flush				();
	void			FlushLine			();
	void			FlushToken			();
	static u8		HexToByte			( u32 c );
	static bool		IsWhitespace		( u32 c );
	void			Parse				();
	u32				PackColor			( const u8* color, u32 colorSize );
	void			SaveCursor			();

public:

	enum {
		LEFT_JUSTIFY,
		RIGHT_JUSTIFY,
		CENTER_JUSTIFY,
	};

	SET ( cc8*, Text, mStr )
	SET ( MOAIFont*, Font, mFont )
	SET ( const USRect&, Frame, mFrame )
	SET ( float, LineSpacing, mLineSpacing )
	SET ( float, Points, mPoints )
	SET ( u32, Alignment, mJustify )

	//----------------------------------------------------------------//
	void			Layout				( MOAITextLayout& layout, cc8* str, MOAITextCursor& cursor );
	void			SetCurves			( MOAIAnimCurve** curves, u32 totalCurves );
					MOAITextFrame		();
	virtual			~MOAITextFrame		();
};

#endif
