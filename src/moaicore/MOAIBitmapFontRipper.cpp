// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <contrib/utf8.h>
#include <moaicore/MOAIBitmapFontRipper.h>
#include <moaicore/MOAIFont.h>

//================================================================//
// MOAIBitmapFontRipper
//================================================================//

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::Clear () {

	this->mFontHeight = 0;
}

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::CopyGlyph ( MOAIBitmapGlyphRipper& glyph, int dstX, int dstY ) {

	int srcX = glyph.mSrcRect.mXMin;
	int srcY = glyph.mSrcRect.mYMin;

	int width = glyph.mSrcRect.Width ();
	int height = glyph.mSrcRect.Height ();

	for ( int y = 0; y < height; ++y ) {
		for ( int x = 0; x < width; ++x ) {
			
			u32 color = ( u32 )this->GetPixel (( u32 )( srcX + x ), ( u32 )( srcY + y ), RGBA );
			this->SetPixel ( dstX + x, dstY + y, color );
		}
	}
}

//----------------------------------------------------------------//
u32 MOAIBitmapFontRipper::GetPixel ( u32 x, u32 y, u32 mask ) {
	UNUSED ( mask );

	return this->mInBmp.GetPixel ( x, y );
}

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::GetGlyphBounds ( MOAIBitmapGlyphRipper& glyph, USIntRect& frame ) {

	glyph.mIsWhitespace = true;
	glyph.mSrcRect = frame;

	USIntRect bounds;

	bounds.mXMin = frame.mXMax;
	bounds.mXMax = frame.mXMin - 1;

	bounds.mYMin = frame.mYMax;
	bounds.mYMax = frame.mYMin - 1;

	for ( int y = frame.mYMin; y < frame.mYMax; ++y ) {
		for ( int x = frame.mXMin; x < frame.mXMax; ++x ) {
			
			u32 color = ( u32 )this->GetPixel (( u32 )x, ( u32 )y, RGBA );
			if ( !( color & 0xff000000 )) continue;
			
			if ( x < bounds.mXMin ) bounds.mXMin = x;
			if ( x >= bounds.mXMax ) bounds.mXMax = x + 1;
			
			if ( y < bounds.mYMin ) bounds.mYMin = y;
			if ( y >= bounds.mYMax ) bounds.mYMax = y + 1;
		}
	}
	
	if (( bounds.mXMin < bounds.mXMax ) && ( bounds.mYMin < bounds.mYMax )) {
		glyph.mIsWhitespace = false;
		glyph.mSrcRect = bounds;
	}
}

//----------------------------------------------------------------//
USIntRect MOAIBitmapFontRipper::GetGlyphFrame ( u32 x, u32 y ) {

	USIntRect rect;
	rect.Init ( 0, 0, 0, 0 );

	int imgWidth = this->mInBmp.GetWidth ();
	int imgHeight = this->mInBmp.GetHeight ();

	int left = x + 1;
	
	int right = left;
	for ( ; right < imgWidth; ++right ) {
		if ( this->mMaskColor == ( u32 )this->GetPixel ( right, y, RGB )) break;
	}
	if ( right == imgWidth ) return rect;

	int top = y;
	for ( ; top > 0; --top ) {
		if ( this->mMaskColor == ( u32 )this->GetPixel ( left, top, RGB )) break;
	}
	if ( top == 0 ) return rect;
	
	int bottom = y;
	for ( ; bottom < imgHeight; ++bottom ) {
		if ( this->mMaskColor == ( u32 )this->GetPixel ( left, bottom, RGB )) break;
	}
	if ( bottom == imgHeight ) return rect;
	
	rect.mXMin = left;
	rect.mXMax = right;
	
	rect.mYMin = top + 1;
	rect.mYMax = bottom;
	
	return rect;
}

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::MakeFont ( MOAIFont& font, cc8* charCodes ) {

	//int x = 1;
	//int y = 1;
	//int nextY = 0;

	//int imgWidth = mOutBmp.GetWidth ();
	//int imgHeight = mOutBmp.GetHeight ();

	//int i = 0;
	//GlyphListIt glyphIt = mGlyphList.begin ();
	//for ( ; charCodes [ i ] && ( glyphIt != mGlyphList.end ()); ++glyphIt ) {
	//	
	//	u32 c = u8_nextchar ( charCodes, &i );
	//	
	//	MOAIBitmapGlyphRipper& glyph = ( *glyphIt );
	//	
	//	int width = glyph.mSrcRect.Width () + 1;
	//	int height = glyph.mSrcRect.Height () + 1;
	//	
	//	if (( x + width ) > imgWidth ) {
	//		x = 1;
	//		y = nextY;
	//	}

	//	if (( y + height ) > imgHeight ) break;
	//	if (( y + height ) > nextY ) nextY = y + height;

	//	CopyGlyph ( glyph, x, y );
	//	
	//	glyph.mGlyph.SetCode ( c );
	//	
	//	glyph.mDestRect.mXMin = x;
	//	glyph.mDestRect.mXMax = x + width - 1;
	//	
	//	glyph.mDestRect.mYMin = y;
	//	glyph.mDestRect.mYMax = y + height - 1;
	//	
	//	this->MakeGlyph ( glyph );
	//	font.SetGlyph ( glyph.mGlyph );
	//	
	//	x += width;
	//}
	//
	//font.SetScale (( float )this->mFontHeight );
}

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::MakeGlyph ( MOAIBitmapGlyphRipper& glyph ) {

	//float imgWidth = ( float )mOutBmp.GetWidth ();
	//float imgHeight = ( float )mOutBmp.GetHeight ();
	//
	//USRect uvRect;

	//uvRect.mXMin = ( float )glyph.mDestRect.mXMin / imgWidth;
	//uvRect.mXMax = ( float )glyph.mDestRect.mXMax / imgWidth;
	//	
	//uvRect.mYMin = ( float )glyph.mDestRect.mYMin / imgHeight;
	//uvRect.mYMax = ( float )glyph.mDestRect.mYMax / imgHeight;
	//
	//float fontHeight = ( float )this->mFontHeight;
	//
	//float width = ( float )glyph.mSrcRect.Width () / fontHeight;
	//float height = ( float )glyph.mSrcRect.Height () / fontHeight;
	//float yOff = ( float )( this->mFontBase - glyph.mBase ) / fontHeight;
	//
	//if ( glyph.mIsWhitespace ) {
	//	glyph.mGlyph.SetScreenRect ( 0.0f, height, yOff  );
	//}
	//else {
	//	glyph.mGlyph.SetUVRect ( uvRect );
	//	glyph.mGlyph.SetScreenRect ( width, height, yOff  );
	//}
	//
	//glyph.mGlyph.SetAdvanceX ( width );
	//glyph.mGlyph.SetBearingX ( 0.0f );
}

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::RipAndReturn ( MOAIFont& font, MOAIImage& image, cc8* charCodes ) {

	//font.Init ( charCodes );
	//if ( !font.Size ()) return;

	//this->mFontHeight = 0;
	//this->mFontBase = 0;

	//if ( !this->mInBmp.IsOK ()) return;

	//this->mOutBmp.Init (
	//	this->mInBmp.GetWidth (),
	//	this->mInBmp.GetHeight (),
	//	this->mInBmp.GetColorFormat (),
	//	this->mInBmp.GetPixelFormat ()
	//);
	//if ( !this->mOutBmp.IsOK ()) return;
	//this->mOutBmp.ClearBitmap ();

	//this->Scan ();
	//this->MakeFont ( font, charCodes );
	//
	//// take over of the output bitmap
	//image.Take ( this->mOutBmp );
	//
	//// done with the bitmaps
	//this->mInBmp.Clear ();
	//this->mOutBmp.Clear ();	
}

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::RipAndReturn ( USData& inData, MOAIFont& font, MOAIImage& image, cc8* charCodes ) {

	this->mInBmp.Load ( inData );
	this->RipAndReturn ( font, image, charCodes );
}

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::RipAndReturn ( cc8* inFile, MOAIFont& font, MOAIImage& image, cc8* charCodes ) {

	this->mInBmp.Load ( inFile );
	this->RipAndReturn ( font, image, charCodes );
}

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::RipGlyph ( u32 x, u32 y ) {

	USIntRect frame = this->GetGlyphFrame ( x, y );

	if ( !frame.Width ()) return;
	if ( !frame.Height ()) return;
	
	this->mGlyphList.push_back ( MOAIBitmapGlyphRipper ());
	MOAIBitmapGlyphRipper& glyphRipper = this->mGlyphList.back ();
	
	this->GetGlyphBounds ( glyphRipper, frame );
	glyphRipper.mBase = y - glyphRipper.mSrcRect.mYMin;
	
	if ( !glyphRipper.mIsWhitespace ) {
	
		int height = glyphRipper.mSrcRect.Height ();
		if ( height > this->mFontHeight ) {
			this->mFontHeight = height;
		}
		
		int base = y - glyphRipper.mSrcRect.mYMin;
		if ( base > this->mFontBase ) {
			this->mFontBase = base;
		}
	}
}

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::Scan () {

	this->mBGColor = ( u32 )this->GetPixel ( 0, 0, RGB );

	for ( u32 y = 0; y < mInBmp.GetHeight (); ++y ) {
		
		u32 color = ( u32 )this->GetPixel ( 0, y, RGB );
		
		if ( color != this->mBGColor ) {
		
			this->mMaskColor = color;
			ScanRow ( y );
		}
	}
}

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::ScanRow ( u32 y ) {

	for ( u32 x = 1; x < ( u32 )mInBmp.GetWidth (); ++x ) {
	
		u32 color = ( u32 )this->GetPixel ( x, y, RGB );
		
		if ( color == this->mMaskColor ) {
			RipGlyph ( x, y );
		}
	}
}

//----------------------------------------------------------------//
void MOAIBitmapFontRipper::SetPixel ( u32 x, u32 y, u32 color ) {

	this->mOutBmp.SetPixel ( x, y, color );
}

//----------------------------------------------------------------//
MOAIBitmapFontRipper::MOAIBitmapFontRipper () {
}

//----------------------------------------------------------------//
MOAIBitmapFontRipper::~MOAIBitmapFontRipper () {
}
