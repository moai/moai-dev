// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USFONTRIPPER_H
#define USFONTRIPPER_H

#include <uslsext/USFont.h>
#include <uslsext/USImage.h>

class USData;

//================================================================//
// USGlyphRipper
//================================================================//
class USGlyphRipper {
private:

	friend class USFontRipper;

	bool		mIsWhitespace;
	USIntRect	mSrcRect;
	USIntRect	mDestRect;
	
	int			mBase;
	
	USGlyph		mGlyph;
};

//================================================================//
// USFontRipper
//================================================================//
class USFontRipper {
private:

	static const u32 RGB	= 0x00ffffff;
	static const u32 RGBA	= 0xffffffff;

	u32 mBGColor;
	u32 mMaskColor;

	int mFontHeight;
	int mFontBase;

	typedef STLList < USGlyphRipper >::iterator GlyphListIt;
	STLList < USGlyphRipper > mGlyphList;
	
	USImage mInBmp;
	USImage mOutBmp;

	//----------------------------------------------------------------//
	void		CopyGlyph		( USGlyphRipper& glyph, int dstX, int dstY );
	void		GetGlyphBounds	( USGlyphRipper& glyph, USIntRect& frame );
	USIntRect	GetGlyphFrame	( u32 x, u32 y );
	u32			GetPixel		( u32 x, u32 y, u32 mask = RGBA );
	void		MakeFont		( USFont& font );
	void		MakeGlyph		( USGlyphRipper& glyph );
	void		RipGlyph		( u32 x, u32 y );
	void		Scan			();
	void		ScanRow			( u32 y );
	void		SetPixel		( u32 x, u32 y, u32 color );
	void		RipAndReturn	( USFont& font, USImage& image, cc8* charCodes );
public:

	//----------------------------------------------------------------//
	void			Clear				();
	void			RipAndReturn		( USData& inData, USFont& font, USImage& image, cc8* charCodes );
	void			RipAndReturn		( cc8* inFile, USFont& font, USImage& image, cc8* charCodes );
					USFontRipper		();
					~USFontRipper		();
};

#endif
