// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBITMAPFONTRIPPER_H
#define MOAIBITMAPFONTRIPPER_H

#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIImage.h>

//================================================================//
// MOAIBitmapGlyphRipper
//================================================================//
class MOAIBitmapGlyphRipper {
private:

	friend class MOAIBitmapFontRipper;

	bool			mIsWhitespace;
	USIntRect		mSrcRect;
	USIntRect		mDestRect;
	
	int				mBase;
	
	MOAIGlyph		mGlyph;
};

//================================================================//
// MOAIBitmapFontRipper
//================================================================//
class MOAIBitmapFontRipper {
private:

	static const u32 RGB	= 0x00ffffff;
	static const u32 RGBA	= 0xffffffff;

	u32 mBGColor;
	u32 mMaskColor;

	int mFontHeight;
	int mFontBase;

	typedef STLList < MOAIBitmapGlyphRipper >::iterator GlyphListIt;
	STLList < MOAIBitmapGlyphRipper > mGlyphList;
	
	MOAIImage mInBmp;
	MOAIImage mOutBmp;

	//----------------------------------------------------------------//
	void			CopyGlyph					( MOAIBitmapGlyphRipper& glyph, int dstX, int dstY );
	void			GetGlyphBounds				( MOAIBitmapGlyphRipper& glyph, USIntRect& frame );
	USIntRect		GetGlyphFrame				( u32 x, u32 y );
	u32				GetPixel					( u32 x, u32 y, u32 mask = RGBA );
	void			MakeFont					( MOAIFont& font, cc8* charCodes );
	void			MakeGlyph					( MOAIBitmapGlyphRipper& glyph );
	void			RipGlyph					( u32 x, u32 y );
	void			Scan						();
	void			ScanRow						( u32 y );
	void			SetPixel					( u32 x, u32 y, u32 color );
	void			RipAndReturn				( MOAIFont& font, MOAIImage& image, cc8* charCodes );
	
public:

	//----------------------------------------------------------------//
	void			Clear						();
	void			RipAndReturn				( USData& inData, MOAIFont& font, MOAIImage& image, cc8* charCodes );
	void			RipAndReturn				( cc8* inFile, MOAIFont& font, MOAIImage& image, cc8* charCodes );
					MOAIBitmapFontRipper		();
					~MOAIBitmapFontRipper		();
};

#endif
