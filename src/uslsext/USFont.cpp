// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslsext/USAnimCurve.h>
#include <uslsext/USFont.h>
#include <uslsext/USGLQuad.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
static u8 _hexdigit ( u32 c ) {

	if (( c >= '0' ) && ( c <= '9' )) return ( u8 )( c - '0' );
	if (( c >= 'a' ) && ( c <= 'f' )) return ( u8 )( c + 10 - 'a' );
	if (( c >= 'A' ) && ( c <= 'F' )) return ( u8 )( c + 10 - 'A' );

	return 0xff;
}

//----------------------------------------------------------------//
static bool _isWhitespace ( u32 c ) {

	if ( !c ) return true;
	if ( c == ' ' ) return true;
	if ( c == '\t' ) return true;
	if ( c == '\n' ) return true;
	
	return false;
}

//================================================================//
// USTextStyler
//================================================================//

//----------------------------------------------------------------//
void USTextStyler::Reset () {

	this->mRGBA = USColor::PackRGBA ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//================================================================//
// USFontStringParser
//================================================================//
class USFontStringParser {
private:

	friend class USFont;
	
	cc8*	mStr;
	u32		mIdx;

	//----------------------------------------------------------------//
	u32			DecodeChar			( u32 idx );
	u32			GetNextChar			();
	u32			GetNextChar			( USTextStyler& styler );
	void		ParseStyles			();
	void		ParseStyles			( USTextStyler& styler );
	void		SetString			( cc8* str );
};

//----------------------------------------------------------------//
u32 USFontStringParser::DecodeChar ( u32 idx ) {

	return this->mStr [ idx ];
}

//----------------------------------------------------------------//
u32 USFontStringParser::GetNextChar () {

	this->ParseStyles ();
	return this->DecodeChar ( this->mIdx++ );
}

//----------------------------------------------------------------//
u32 USFontStringParser::GetNextChar ( USTextStyler& styler ) {

	this->ParseStyles ( styler );
	return this->DecodeChar ( this->mIdx++ );
}

//----------------------------------------------------------------//
void USFontStringParser::ParseStyles () {

	u32 cursor = this->mIdx;
	u32 c;

	c = this->DecodeChar ( cursor++ );
	if ( c != '<' ) return;
	
	while ( c != '>' ) {
		c = this->DecodeChar ( cursor++ );
	}
	this->mIdx = cursor;
}

//----------------------------------------------------------------//
void USFontStringParser::ParseStyles ( USTextStyler& styler ) {

	u32 cursor = this->mIdx;
	u32 c;

	c = this->DecodeChar ( cursor++ );
	if ( c != '<' ) return;
	
	enum {
		START,
		FINISH,
		ERROR,
		STOP,
		
		COLOR_START,
		COLOR_BODY,
		COLOR_FINISH,
		
		COLOR_MAX = 8,
		
		COLOR_GRAY_16		= 1,
		COLOR_GRAY_256		= 2,
		COLOR_RGB_16		= 3,
		COLOR_RGBA_16		= 4,
		COLOR_UNKNOWN		= 5,
		COLOR_RGB_256		= 6,
		COLOR_RGBA_256_16	= 7,
		COLOR_RGBA_256		= 8,
	};
	
	bool hasColor = false;
	u32 rgba = 0xffffffff;
	u8 color [ COLOR_MAX ];
	u32 colorSize = 0;
	
	u32 state = START;
	while ( state != STOP ) {
		
		switch ( state ) {
		
			case START: {
				
				c = this->DecodeChar ( cursor++ );
				if ( _isWhitespace ( c )) continue;
				
				state = c == 'c' ? COLOR_START : ERROR;
				break;
			}
			
			case ERROR: {
				c = this->DecodeChar ( cursor++ );
				if ( c == '>' ) {
					state = STOP;
				}
				break;
			}
			
			case FINISH: {
				
				if ( hasColor ) {
					styler.mRGBA = rgba;
				}
				this->mIdx = cursor;
				state = STOP;
				break;
			}
			
			case COLOR_START: {
			
				c = this->DecodeChar ( cursor++ );
				if ( _isWhitespace ( c )) continue;
				
				if ( c == ':' ) {
					state = COLOR_BODY;
					continue;
				}
				
				if ( c == '>' ) {
					styler.Reset ();
					state = FINISH;
					continue;
				}
				
				state = ERROR;
				break;
			}
			
			case COLOR_BODY: {
				
				c = this->DecodeChar ( cursor++ );
				if ( _isWhitespace ( c )) continue;
				
				if ( c == '>' ) {
					state = COLOR_FINISH;
					continue;
				}
				
				u8 hex = _hexdigit ( c );
				if (( hex != 0xff ) && ( colorSize < COLOR_MAX )) {
					color [ colorSize++ ] = hex;
					continue;
				}
				
				state = ERROR;
				break;
			}
			
			case COLOR_FINISH: {
				
				if (( colorSize == 0 ) || ( colorSize == COLOR_UNKNOWN )) {
					state = ERROR;
					continue;
				}
				
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
				}
				
				hasColor = true;
				state = ( c == '>' ) ? FINISH : START;
				break;
			}
		}
	}
}

//----------------------------------------------------------------//
void USFontStringParser::SetString ( cc8* str ) {

	this->mStr = str;
	this->mIdx = 0;
}

//================================================================//
// USGlyph
//================================================================//

//----------------------------------------------------------------//
void USGlyph::Draw ( float points, float x, float y ) {

	if ( this->mWidth ) {

		USGLQuad glQuad;
		
		x += ( this->mBearingX * points );
		y += ( this->mYOff * points ); 

		glQuad.SetVerts (
			x,
			y,
			x + ( this->mWidth * points ),
			y + ( this->mHeight * points )
		);

		glQuad.SetUVs ( this->mUVRect );
		glQuad.Draw ();
	}
}

//----------------------------------------------------------------//
USKernVec USGlyph::GetKerning ( u32 name ) {

	u32 total = this->mKernTable.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		USKernVec& kernVec = this->mKernTable [ i ];
		
		if ( kernVec.mName == name ) {
			return kernVec;
		}
	}
	
	USKernVec kernVec;
	kernVec.mName = 0xffffffff;
	kernVec.mX = 0.0f;
	kernVec.mY = 0.0f;
	
	return kernVec;
}

//----------------------------------------------------------------//
void USGlyph::ReserveKernTable ( u32 total ) {

	this->mKernTable.Init ( total );
}

//----------------------------------------------------------------//
void USGlyph::SetKernVec ( u32 id, const USKernVec& kernVec ) {

	this->mKernTable [ id ] = kernVec;
}

//----------------------------------------------------------------//
void USGlyph::SetScreenRect ( float width, float height, float yOff ) {
	
	this->mWidth = width;
	this->mHeight = height;
	this->mYOff = yOff;
}

//----------------------------------------------------------------//
USGlyph::USGlyph () :
	mWidth ( 0 ),
	mAdvanceX ( 0 ) {
}

//----------------------------------------------------------------//
USGlyph::~USGlyph () {
}

//================================================================//
// USGlyphBuffer
//================================================================//

//----------------------------------------------------------------//
void USGlyphBuffer::PushGlyph ( u32 glyphID, float x, float y, float points, u32 rgba ) {

	USGlyphSprite glyphSprite;
	
	glyphSprite.mGlyphID = glyphID;
	glyphSprite.mX = x;
	glyphSprite.mY = y;
	glyphSprite.mRGBA = rgba;
	glyphSprite.mPoints = points;

	this->Push ( glyphSprite );
}

//================================================================//
// USFont
//================================================================//

//----------------------------------------------------------------//
void USFont::Draw ( const USGlyphBuffer& layout, u32 reveal ) {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
	USGLQuad::BindVertexFormat ( drawBuffer );

	USColorVec baseColor = drawBuffer.GetPenColor ();
	USColorVec blendColor;
	u32 rgba = 0xffffffff;

	u32 size = layout.GetTop ();
	for ( u32 i = 0; ( i < size ) && ( i < reveal ); ++i ) {
		const USGlyphSprite& sprite = layout [ i ];
		
		if ( sprite.mRGBA != rgba ) {
			rgba = sprite.mRGBA;
			
			blendColor.SetRGBA ( rgba );
			blendColor.Modulate ( baseColor );
			drawBuffer.SetPenColor ( blendColor );
		}
		USGlyph& glyph = this->mGlyphs [ sprite.mGlyphID ];
		glyph.Draw ( sprite.mPoints, sprite.mX, sprite.mY );
	}
}

//----------------------------------------------------------------//
void USFont::DrawGlyph ( u32 c, float points, float x, float y ) {

	USGlyph& glyph = this->GetGlyphForChar ( c );
	glyph.Draw ( points, x, y );
}

//----------------------------------------------------------------//
void USFont::EndDrawing () {
}

//----------------------------------------------------------------//
USGlyph& USFont::GetGlyph ( u32 id ) {

	return this->mGlyphs [ id ];
}

//----------------------------------------------------------------//
USGlyph& USFont::GetGlyphForChar ( u32 c ) {

	u32 id = this->mGlyphMap [ c ];
	
	u32 totalGlyphs = this->mGlyphs.Size ();
	if ( totalGlyphs ) {
	
		if ( id < totalGlyphs ) {
			return this->mGlyphs [ id ];
		}
		return this->mGlyphs [ 0 ];
	}
	
	static USGlyph nullGlyph;
	return nullGlyph;
}

//----------------------------------------------------------------//
USTextLine USFont::GetLine ( cc8* str, float points, float width ) {

	USTextLine line;
	
	line.mIsFinal = false;
	
	line.mLength = 0;
	line.mVisibleLength = 0;
	line.mNonWhitespace = 0;
	
	line.mAdvance = 0.0f;
	line.mWidth = 0.0f;

	bool more = true;
	bool over = false;
	bool first = true;
	
	while ( more ) {
		
		USTextToken token = this->GetToken ( &str [ line.mLength ], points, 0 );
		
		line.mAdvance += token.mKern;
		
		if (( line.mAdvance + token.mWidth ) > width ) {
			over = true;
		}
		
		switch ( token.mType ) {
			
			case USTextToken::kEOF: {
				line.mIsFinal = true;
				more = false;
				break;
			}
			
			case USTextToken::kNewline: {
				line.mLength += token.mLength;
				more = false;
				break;
			}
			
			case USTextToken::kWhitespace: {
				line.mLength += token.mLength;
				line.mAdvance += token.mAdvance;
				break;
			}
			
			case USTextToken::kWord: {
			
				if ( over && ( !first )) {
					more = false;
				}
				else {
					line.mLength += token.mLength;
					line.mVisibleLength = line.mLength;
					line.mNonWhitespace += token.mNonWhitespace;
					line.mAdvance += token.mAdvance;
					line.mWidth = line.mAdvance;
				}
				break;
			}
		}
		first = false;
	}
	return line;
}

//----------------------------------------------------------------//
USTextToken USFont::GetToken ( cc8* str, float points, USTextToken* prevToken ) {

	USFontStringParser parser;
	parser.SetString ( str );

	USTextToken token;
	token.mLastGlyph = 0;
	
	token.mLength = 0;
	token.mNonWhitespace = 0;
	token.mType = USTextToken::kEOF;
	
	token.mAdvance = 0.0f;
	token.mWidth = 0.0f;
	token.mKern = 0.0f;
	
	u32 c = parser.GetNextChar ();
	token.mLength = 1;
	
	// EOF, again
	if ( c == 0 ) {
		return token;
	}
	
	// Newline
	if ( c == '\n' ) {
		token.mType = USTextToken::kNewline;
		return token;
	}
	
	// gotta handle the first glyph...
	USGlyph& firstGlyph = this->GetGlyphForChar ( c );
	token.mLastGlyph = &this->GetGlyphForChar ( c );
	
	// figure kerning
	if ( prevToken && prevToken->mLastGlyph ) {
		USKernVec kernVec = prevToken->mLastGlyph->GetKerning ( firstGlyph.mName );
		token.mKern = kernVec.mX * points;
	}
	
	// WS
	if ( _isWhitespace ( c )) {
		token.mAdvance = firstGlyph.mAdvanceX * points;
		token.mType = USTextToken::kWhitespace;
		return token;
	}
	
	float x = 0.0f;
	USGlyph* prevGlyph = 0;
	
	// word
	while ( !_isWhitespace ( c )) {
		
		token.mNonWhitespace++;
		
		USGlyph& glyph = this->GetGlyphForChar ( c );
		token.mLastGlyph = &glyph;
		
		if ( prevGlyph ) {
			USKernVec kernVec = prevGlyph->GetKerning ( glyph.mName );
			x += kernVec.mX * points;
		}
		
		token.mWidth = x + (( glyph.mWidth + glyph.mBearingX ) * points );
		
		x += glyph.mAdvanceX * points;
		token.mAdvance = x;
		
		prevGlyph = &glyph;
		c = parser.GetNextChar ();
	}
	
	token.mLength = parser.mIdx - 1;
	token.mType = USTextToken::kWord;
	return token;
}

//----------------------------------------------------------------//
void USFont::Init ( u32 totalGlyphs ) {
	
	this->mGlyphs.Init ( totalGlyphs );
}

//----------------------------------------------------------------//
float USFont::Justify ( float x, float width, float lineWidth, u32 justify ) {
	
	switch ( justify ) {
		case CENTER_JUSTIFY:
			return ( x + ( width * 0.5f )) - ( lineWidth * 0.5f );
			
		case LEFT_JUSTIFY:
			return x;

		case RIGHT_JUSTIFY:
			return width - lineWidth;
	}
	
	return x;
}

//----------------------------------------------------------------//
//void USFont::LayoutLine ( USGlyphBuffer& glyphBuffer, cc8* str, float points, float x, float y ) {
//	
//	if ( !this->mGlyphs.Size ()) return;
//	
//	USGlyph* prevGlyph = 0;
//	
//	USFontStringParser parser;
//	parser.SetString ( str );
//	
//	for ( u32 i = 0; str [ i ]; ++i ) {
//		
//		u32 c = parser.GetNextChar ();
//		USGlyph& glyph = this->GetGlyphForChar ( c );
//		
//		if ( prevGlyph ) {
//			USKernVec kernVec = prevGlyph->GetKerning ( glyph.mName );
//			x += kernVec.mX * points;
//		}
//		
//		glyph.Draw ( points, x, y );
//		x += ( glyph.mAdvanceX * points );
//		
//		prevGlyph = &glyph;
//	}
//}

//----------------------------------------------------------------//
u32 USFont::Layout ( USGlyphBuffer& glyphBuffer, cc8* str, float points, USRect frame, u32 justify, USTextStyler& styler, USAnimCurve** curves, u32 totalCurves ) {
	
	frame.Bless ();
	
	float width = frame.Width ();
	float height = frame.Height ();
	
	float lineSpacing = this->GetLineSpacing () * points;
	float y = frame.mYMin;
	
	u32 totalLines = ( u32 )ceilf ( height / lineSpacing );
	u32 cursor = 0;
	
	for ( u32 i = 0; i < totalLines; ++i ) {
		
		USTextLine line = this->GetLine ( &str [ cursor ], points, width );
		float x = this->Justify ( frame.mXMin, width, line.mWidth, justify );
		
		USAnimCurve* curve = 0;
		if ( curves ) {
			curve = curves [ i % totalCurves ];
		}
		
		this->LayoutLine (
			glyphBuffer,
			&str [ cursor ],
			points,
			line.mVisibleLength,
			x,
			y,
			styler,
			curve,
			( u32 )width,
			( u32 )( x - frame.mXMin )
		);
		
		y += lineSpacing;
		cursor += line.mLength;
		
		if ( line.mIsFinal ) {
			break;
		}
	}
	return cursor;
}

//----------------------------------------------------------------//
void USFont::LayoutLine ( USGlyphBuffer& glyphBuffer, cc8* str, float points, u32 size, float x, float y, USTextStyler& styler, USAnimCurve* curve, u32 width, u32 xOff ) {
	
	if ( !this->mGlyphs.Size ()) return;
	
	float penX = 0.0f;
	USGlyph* prevGlyph = 0;
	
	USFontStringParser parser;
	parser.SetString ( str );
	
	while ( parser.mIdx < size ) {
		
		u32 c = parser.GetNextChar ( styler );
		
		u32 id = this->mGlyphMap [ c ];
		USGlyph& glyph = this->mGlyphs [ id ];
		
		if ( prevGlyph ) {
			USKernVec kernVec = prevGlyph->GetKerning ( glyph.mName );
			penX += kernVec.mX * points;
		}
		
		float yOffset = 0.0f;
		if ( curve ) {
			yOffset = curve->GetFloatValue (( penX + xOff ) / width );
		}
		
		if ( glyph.mWidth ) {
			glyphBuffer.PushGlyph ( id, x + penX, y + yOffset, points, styler.mRGBA );
		}
		penX += ( glyph.mAdvanceX * points );
		
		prevGlyph = &glyph;
	}
}

//----------------------------------------------------------------//
void USFont::SetCharGlyphID ( u32 c, u32 id ) {

	this->mGlyphMap [ c ] = ( u8 )id;
}

//----------------------------------------------------------------//
void USFont::SetGlyph ( u32 id, const USGlyph& glyph ) {

	this->mGlyphs [ id ] = glyph;
}

//----------------------------------------------------------------//
void USFont::SetGlyphMap ( cc8* mapString ) {

	for ( u32 i = 0; mapString [ i ]; ++i ) {
	
		char c = mapString [ i ];
		this->mGlyphMap [( u32 )c ] = ( u8 )i;
	}
}

//----------------------------------------------------------------//
USFont::USFont () :
	mScale ( 1.0f ),
	mLineSpacing ( 1.0f ) {
}

//----------------------------------------------------------------//
USFont::~USFont () {
}
