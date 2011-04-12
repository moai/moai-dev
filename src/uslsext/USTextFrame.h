// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USTEXTFRAME_H
#define	USTEXTFRAME_H

#include <uslsext/USFont.h>

class USAnimCurve;
class USGlyph;
class USTextLayout;

//================================================================//
// USTextCursor
//================================================================//
class USTextCursor {
private:

	friend class USTextFrame;

	u32		mIndex;
	u32		mRGBA;

public:

	GET ( u32, Index, mIndex )
	GET ( u32, RGBA, mRGBA )
	
	//----------------------------------------------------------------//
	void	Reset	();
};

//================================================================//
// USTextFrame
//================================================================//
class USTextFrame {
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

	int				mIdx;
	u32				mRGBA;
	USTextCursor*	mCursor;
	
	
	u32				mLineBottom;
	u32				mLineTop;
	
	float			mLineHeight;
	u32				mLineCount;
	u32				mTotalLines;
	
	float			mLineXMax;
	float			mTokenXMin;
	float			mTokenXMax;
	
	USVec2D			mPen;
	cc8*			mStr;
	USFont*			mFont;
	USRect			mFrame;
	float			mPoints;
	u32				mJustify;
	USTextLayout*	mLayout;
	const USGlyph*	mGlyph;
	
	USAnimCurve**	mCurves;
	u32				mTotalCurves;

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
	SET ( USFont*, Font, mFont )
	SET ( const USRect&, Frame, mFrame )
	SET ( float, Points, mPoints )
	SET ( u32, Alignment, mJustify )

	//----------------------------------------------------------------//
	void			Layout				( USTextLayout& layout, cc8* str, USTextCursor& cursor );
	void			SetCurves			( USAnimCurve** curves, u32 totalCurves );
					USTextFrame			();
	virtual			~USTextFrame		();
};

#endif
