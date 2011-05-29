// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USAnimCurve.h>
#include <uslsext/USFont.h>
#include <uslsext/USTextFrame.h>
#include <uslsext/USTextLayout.h>
#include <contrib/utf8.h>

//================================================================//
// USTextCursor
//================================================================//

//----------------------------------------------------------------//
void USTextCursor::Reset () {

	this->mIndex	= 0;
	this->mRGBA		= 0xffffffff;
}

//================================================================//
// USTextFrame
//================================================================//

#define TRANSITION(next)					\
	state = next;							\
	break;

#define TRANSITION_ON_MATCH(match,next)		\
	if ( c == match ) {						\
		TRANSITION ( next )					\
	}

//----------------------------------------------------------------//
u32 USTextFrame::DecodeChar () {

	if ( this->mStr [ this->mIdx ]) {
		return u8_nextchar ( this->mStr, &this->mIdx );
	}
	++this->mIdx;
	return 0;
}

//----------------------------------------------------------------//
void USTextFrame::Flush () {

	this->FlushToken ();
	this->FlushLine ();
}

//----------------------------------------------------------------//
void USTextFrame::FlushLine () {

	if ( this->mLineCount >= this->mTotalLines ) return;

	float x = this->mFrame.mXMin;
	float width = this->mFrame.Width ();
	float lineHeight = this->mFont->GetLineSpacing () * this->mPoints;

	switch ( this->mJustify ) {
		
		case CENTER_JUSTIFY:
			x = ( x + ( width * 0.5f )) - ( this->mLineXMax * 0.5f );
			
		case LEFT_JUSTIFY:
			break;

		case RIGHT_JUSTIFY:
			x = this->mFrame.mXMax - this->mLineXMax;
	}
	
	USAnimCurve* curve = 0;
	if ( this->mCurves ) {
		curve = this->mCurves [ this->mLineCount % this->mTotalCurves ];
	}
	
	for ( u32 i = this->mLineBottom; i < this->mLineTop; ++i ) {
		
		USTextSprite& sprite = ( *this->mLayout )[ i ];
		
		float yOff = 0.0f;
		if ( curve ) {
			yOff = curve->GetFloatValue ( sprite.mX / width );
		}

		sprite.mX += x;
		sprite.mY = this->mPen.mY + yOff;
	}
	
	this->mLineBottom = this->mLineTop;
	this->mLineXMax = 0.0f;
	this->mPen.mY += lineHeight;
	this->mLineCount++;
	this->mGlyph = 0;
	
	// move token to origin
	u32 tokenTop = this->mLayout->GetTop ();
	for ( u32 i = this->mLineTop; i < tokenTop; ++i ) {
		
		USTextSprite& sprite = ( *this->mLayout )[ i ];
		sprite.mX -= this->mTokenXMin;
	}

	//this->mPen.mX -= this->mTokenXMin;
	this->mPen.mX -= this->mTokenXMin;
	this->mTokenXMax -= this->mTokenXMin;
	this->mTokenXMin = 0.0f;
}

//----------------------------------------------------------------//
void USTextFrame::FlushToken () {
	
	float width = this->mFrame.Width ();
	
	if (( this->mLineXMax > 0.0f ) && ( this->mTokenXMax >= width )) {
		this->FlushLine ();
	}
	
	if ( this->mLineCount < this->mTotalLines ) {
		this->mLineTop = this->mLayout->GetTop ();
		this->mLineXMax = this->mTokenXMax;
	}
}

//----------------------------------------------------------------//
u8 USTextFrame::HexToByte ( u32 c ) {

	if (( c >= '0' ) && ( c <= '9' )) return ( u8 )( c - '0' );
	if (( c >= 'a' ) && ( c <= 'f' )) return ( u8 )( c + 10 - 'a' );
	if (( c >= 'A' ) && ( c <= 'F' )) return ( u8 )( c + 10 - 'A' );

	return 0xff;
}

//----------------------------------------------------------------//
bool USTextFrame::IsWhitespace ( u32 c ) {

	if ( !c ) return true;
	if ( c == ' ' ) return true;
	if ( c == '\t' ) return true;
	if ( c == '\n' ) return true;
	
	return false;
}

//----------------------------------------------------------------//
void USTextFrame::Layout ( USTextLayout& layout, cc8* str, USTextCursor& cursor ) {

	if ( !this->mFont ) return;

	this->mLayout = &layout;
	this->mStr = str;
	
	this->mIdx = ( int )cursor.mIndex;
	this->mRGBA = cursor.mRGBA;
	
	this->mCursor = &cursor;
	
	this->Parse ();
}

//----------------------------------------------------------------//
void USTextFrame::Parse () {
	
	this->mLineBottom = 0;
	this->mLineTop = 0;
	
	this->mLineCount = 0;
	this->mLineXMax = 0.0f;
	this->mTokenXMin = 0.0f;
	this->mTokenXMax = 0.0f;
	
	float points = this->mPoints;
	
	this->mLineHeight = this->mFont->GetLineSpacing () * this->mPoints;
	this->mTotalLines = ( u32 )floorf ( this->mFrame.Height () / this->mLineHeight );
	if ( !this->mTotalLines ) return;

	this->mPen.Init ( 0.0f, this->mFrame.mYMin );

	u32 colorSize = 0;
	u8 color [ COLOR_MAX ];

	u32 c = 0;
	this->mGlyph = 0;

	bool inToken = false;
	int resetIdx = 0;
	
	u32 state = META_START;
	while ( state != META_FINISH ) {
		
		switch ( state ) {
			
			//================================================================//
			// META
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			// check to see if we've encountered a style escape
			case META_START: {
				
				resetIdx = this->mIdx;
				c = this->DecodeChar ();

				if ( c == 0 ) {
					
					this->FlushToken ();
					
					if ( this->mLineCount >= this->mTotalLines ) {
						TRANSITION ( META_FINISH );
					}
					
					// save the cursor just before the EOF
					this->mCursor->mIndex = resetIdx;
					this->mCursor->mRGBA  = this->mRGBA;
					
					this->FlushLine ();
					
					TRANSITION ( META_FINISH );
				}
				
				TRANSITION_ON_MATCH ( '<', STYLE_START );
				TRANSITION ( TEXT_START );
			}
			
			//================================================================//
			// TEXT
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			// process a single char as text (ignore style escape)
			case TEXT_START: {
				
				if ( c == '\n' ) {
					
					this->FlushToken ();
					inToken = false;
					
					if ( this->mLineCount >= this->mTotalLines ) {
						TRANSITION ( META_FINISH );
					}
					
					this->mCursor->mIndex = resetIdx;
					this->mCursor->mRGBA  = this->mRGBA;
					
					this->FlushLine ();
					
					this->mPen.mX = 0.0f;
					this->mTokenXMax = 0.0f;
					this->mTokenXMin = 0.0f;
					this->mGlyph = 0;
					
					TRANSITION ( META_START );
				}
				
				const USGlyph* prevGlyph = this->mGlyph;
				const USGlyph* glyph = &this->mFont->GetGlyphForChar ( c );
				this->mGlyph = glyph;
				
				assert ( glyph );
				
				bool hasWidth = ( glyph->mWidth > 0.0f );
				
				// apply kerning
				if ( prevGlyph ) {
					USKernVec kernVec = prevGlyph->GetKerning ( glyph->mCode );
					this->mPen.mX += kernVec.mX * points;
				}
				
				if ( hasWidth ) {
	
					if ( inToken == false ) {
						
						// save the cursor just before the new token
						this->mCursor->mIndex = resetIdx;
						this->mCursor->mRGBA  = this->mRGBA;
						
						this->mTokenXMin = this->mPen.mX;
						this->mTokenXMax = this->mPen.mX;
						inToken = true;
					}
					
					// push the glyph
					this->mLayout->PushGlyph ( glyph, resetIdx, this->mPen.mX, 0.0f, points, this->mRGBA );
					this->mTokenXMax = this->mPen.mX + (( glyph->mWidth + glyph->mBearingX ) * points );
				}
				else if ( inToken ) {
				
					this->FlushToken ();
					
					// bail if line count overflow
					if ( this->mLineCount >= this->mTotalLines ) {
						TRANSITION ( META_FINISH );
					}
					inToken = false;
				}
				
				// advance the pen
				this->mPen.mX += glyph->mAdvanceX * points;
				
				// back to start
				TRANSITION ( META_START );
			}
			
			//================================================================//
			// STYLE
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			// see which style we're parsing
			case STYLE_START: {
				
				c = this->DecodeChar ();
				TRANSITION_ON_MATCH ( 'c', COLOR_START );
				TRANSITION ( STYLE_ABORT );
			}
			
			// reset the cursor and go directly to text mode
			case STYLE_ABORT: {
			
				this->mIdx = resetIdx;
				c = this->DecodeChar ();
				TRANSITION ( TEXT_START );
			}
			
			//================================================================//
			// COLOR
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			case COLOR_START: {
			
				colorSize = 0;
			
				c = this->DecodeChar ();
				if ( IsWhitespace ( c )) continue;
				
				if ( c == ':' ) {
					state = COLOR_BODY;
					break;
				}
				
				TRANSITION_ON_MATCH ( ':', COLOR_BODY );
				TRANSITION_ON_MATCH ( '>', COLOR_FINISH );
				
				TRANSITION ( STYLE_ABORT );
			}
			
			//----------------------------------------------------------------//
			case COLOR_BODY: {
				
				c = this->DecodeChar ();
				if ( IsWhitespace ( c )) continue;
				
				TRANSITION_ON_MATCH ( '>', COLOR_FINISH );
				
				u8 hex = HexToByte ( c );
				if (( hex != 0xff ) && ( colorSize < COLOR_MAX )) {
					color [ colorSize++ ] = hex;
					break;
				}
				
				TRANSITION ( STYLE_ABORT );
			}
			
			//----------------------------------------------------------------//
			case COLOR_FINISH: {
				
				this->mRGBA = this->PackColor ( color, colorSize );
				TRANSITION ( META_START );
			}
		}
	}
	
	// discard overflow
	this->mLayout->SetTop ( this->mLineTop );
}

//----------------------------------------------------------------//
u32 USTextFrame::PackColor ( const u8* color, u32 colorSize ) {

	u32 rgba = 0xffffffff;

	switch ( colorSize ) {
					
		case COLOR_GRAY_16: {
			u8 v = ( color [ 0 ] << 4 ) + color [ 0 ];
			rgba = USColor::PackRGBA ( v, v, v, 0xff );
			break;
		}
		case COLOR_GRAY_256: {
			u8 v = ( color [ 0 ] << 4 ) + color [ 1 ];
			rgba = USColor::PackRGBA ( v, v, v, 0xff );
			break;
		}
		case COLOR_RGB_16: {
			rgba = USColor::PackRGBA (
				( color [ 0 ] << 4 ) + color [ 0 ],
				( color [ 1 ] << 4 ) + color [ 1 ],
				( color [ 2 ] << 4 ) + color [ 2 ],
				0xff
			);
			break;
		}
		case COLOR_RGBA_16: {
			rgba = USColor::PackRGBA (
				( color [ 0 ] << 4 ) + color [ 0 ],
				( color [ 1 ] << 4 ) + color [ 1 ],
				( color [ 2 ] << 4 ) + color [ 2 ],
				( color [ 3 ] << 4 ) + color [ 3 ]
			);
			break;
		}
		case COLOR_RGB_256: {
			rgba = USColor::PackRGBA (
				( color [ 0 ] << 4 ) + color [ 1 ],
				( color [ 2 ] << 4 ) + color [ 3 ],
				( color [ 4 ] << 4 ) + color [ 5 ],
				0xff
			);
			break;
		}
		case COLOR_RGBA_256_16: {
			rgba = USColor::PackRGBA (
				( color [ 0 ] << 4 ) + color [ 1 ],
				( color [ 2 ] << 4 ) + color [ 3 ],
				( color [ 4 ] << 4 ) + color [ 5 ],
				( color [ 6 ] << 4 )
			);
			break;
		}
		case COLOR_RGBA_256: {
			rgba = USColor::PackRGBA (
				( color [ 0 ] << 4 ) + color [ 1 ],
				( color [ 2 ] << 4 ) + color [ 3 ],
				( color [ 4 ] << 4 ) + color [ 5 ],
				( color [ 6 ] << 4 ) + color [ 7 ]
			);
			break;
		}
		default:
			break;
	}
	
	return rgba;
}

//----------------------------------------------------------------//
void USTextFrame::SaveCursor () {

	this->mCursor->mIndex = ( u32 )this->mIdx;
	this->mCursor->mRGBA =	this->mRGBA;
}

//----------------------------------------------------------------//
void USTextFrame::SetCurves ( USAnimCurve** curves, u32 totalCurves ) {

	this->mCurves = curves;
	this->mTotalCurves = totalCurves;
}

//----------------------------------------------------------------//
USTextFrame::USTextFrame () :
	mIdx ( 0 ),
	mStr ( 0 ),
	mFont ( 0 ),
	mJustify ( LEFT_JUSTIFY ),
	mCurves ( 0 ),
	mTotalCurves ( 0 ) {
	
	this->mFrame.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
USTextFrame::~USTextFrame () {
}

