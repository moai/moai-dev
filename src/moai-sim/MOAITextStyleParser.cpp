// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/moai_utf8.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleParser.h>
#include <moai-sim/MOAITextStyleCache.h>
#include <moai-sim/MOAITextStyleMap.h>

//================================================================//
// MOAITextStyleParser
//================================================================//

#define TRANSITION(next)					\
	state = next;							\
	break;

#define TRANSITION_ON_MATCH(match,next)		\
	if ( c == match ) {						\
		TRANSITION ( next )					\
	}

//----------------------------------------------------------------//
void MOAITextStyleParser::BuildStyleMap ( MOAITextStyleMap& styleMap, MOAITextStyleCache& styleCache, cc8* str ) {
	
	MOAITextStyle* defaultStyle = styleCache.GetStyle ();
	if ( !( defaultStyle && defaultStyle->mFont )) return;
	
	this->mDefaultStyle = defaultStyle;
	
	this->mIdx = 0;
	this->mPrev = 0;
	this->mStyleCache = &styleCache;
	this->mStyleMap = &styleMap;
	this->mStr = str;
	
	this->mTokenBase = 0;
	this->mTokenTop = 0;
	
	this->PushStyle ( defaultStyle );
	this->Parse ();
}

//----------------------------------------------------------------//
void MOAITextStyleParser::FinishToken () {

	if ( this->mCurrentStyle && ( this->mTokenBase < this->mTokenTop )) {
		this->mStyleMap->PushStyleSpan ( this->mTokenBase, this->mTokenTop, *this->mCurrentStyle );
	}
	
	this->mTokenBase = this->mIdx;
	this->mTokenTop = this->mIdx;
}

//----------------------------------------------------------------//
u32 MOAITextStyleParser::GetChar () {

	this->mPrev = this->mIdx;
	if ( this->mStr [ this->mIdx ]) {
		return moai_u8_nextchar ( this->mStr, &this->mIdx );
	}
	++this->mIdx;
	return 0;
}

//----------------------------------------------------------------//
MOAITextStyleParser::MOAITextStyleParser () :
	mIdx ( 0 ),
	mStr ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextStyleParser::~MOAITextStyleParser () {
}

//----------------------------------------------------------------//
u32 MOAITextStyleParser::PackColor ( const u8* color, u32 colorSize ) {

	u32 rgba = 0xffffffff;

	switch ( colorSize ) {
					
		case COLOR_GRAY_16: {
			u8 v = ( color [ 0 ] << 4 ) + color [ 0 ];
			rgba = ZLColor::PackRGBA ( v, v, v, 0xff );
			break;
		}
		case COLOR_GRAY_256: {
			u8 v = ( color [ 0 ] << 4 ) + color [ 1 ];
			rgba = ZLColor::PackRGBA ( v, v, v, 0xff );
			break;
		}
		case COLOR_RGB_16: {
			rgba = ZLColor::PackRGBA (
				( u8 )( color [ 0 ] << 4 ) + color [ 0 ],
				( u8 )( color [ 1 ] << 4 ) + color [ 1 ],
				( u8 )( color [ 2 ] << 4 ) + color [ 2 ],
				0xff
			);
			break;
		}
		case COLOR_RGBA_16: {
			rgba = ZLColor::PackRGBA (
				( color [ 0 ] << 4 ) + color [ 0 ],
				( color [ 1 ] << 4 ) + color [ 1 ],
				( color [ 2 ] << 4 ) + color [ 2 ],
				( color [ 3 ] << 4 ) + color [ 3 ]
			);
			break;
		}
		case COLOR_RGB_256: {
			rgba = ZLColor::PackRGBA (
				( color [ 0 ] << 4 ) + color [ 1 ],
				( color [ 2 ] << 4 ) + color [ 3 ],
				( color [ 4 ] << 4 ) + color [ 5 ],
				0xff
			);
			break;
		}
		case COLOR_RGBA_256_16: {
			rgba = ZLColor::PackRGBA (
				( color [ 0 ] << 4 ) + color [ 1 ],
				( color [ 2 ] << 4 ) + color [ 3 ],
				( color [ 4 ] << 4 ) + color [ 5 ],
				( color [ 6 ] << 4 )
			);
			break;
		}
		case COLOR_RGBA_256: {
			rgba = ZLColor::PackRGBA (
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
void MOAITextStyleParser::Parse () {
	
	u32 c = 0;

	u32 state = TOKEN_TEXT;
	while ( state != DONE ) {
		
		switch ( state ) {
			
			//================================================================//
			// TOKEN
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			// check to see if we've encountered a style escape
			case TOKEN_TEXT: {
				
				if ( this->ParseStyle ()) {
					TRANSITION ( TOKEN_TEXT );
				}
				
				c = this->GetChar ();

				if ( c == 0 ) {
					this->FinishToken ();
					TRANSITION ( DONE );
				}
				
				this->mCurrentStyle->AffirmGlyph ( c );
				this->mTokenTop = this->mIdx;
				TRANSITION ( TOKEN_TEXT );
			}
		}
	}
	
	u32 totalActiveStyles = this->mActiveStyles.GetTop ();
	for ( u32 i = 0; i < totalActiveStyles; ++i ) {
		MOAITextStyle* style = this->mActiveStyles [ i ];
		assert ( style->mFont );
		style->mFont->ProcessGlyphs ();
	}
}

//----------------------------------------------------------------//
bool MOAITextStyleParser::ParseStyle () {

	if ( this->mStr [ this->mIdx ] != '<' ) return false;

	u32 colorSize = 0;
	u8 color [ COLOR_MAX ];

	u32 c = 0;
	
	int startIdx = this->mIdx;
	
	u32 state = STYLE_START;
	while ( state != DONE ) {
		
		switch ( state ) {

			//================================================================//
			// STYLE
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			// see which style we're parsing
			case STYLE_START: {
				
				c = this->GetChar ();
				
				TRANSITION_ON_MATCH ( '<', STYLE_BODY );
				TRANSITION ( STYLE_ABORT );
			}
			
			//----------------------------------------------------------------//
			case STYLE_BODY: {
				
				c = this->GetChar ();

				if ( c == '<' ) {
					this->mIdx = startIdx + 1;
					this->FinishToken ();
					startIdx = this->mIdx;
					TRANSITION ( DONE );
				}
				
				TRANSITION_ON_MATCH ( '/', STYLE_POP_START );
				TRANSITION_ON_MATCH ( '>', STYLE_POP_FINISH );
				TRANSITION_ON_MATCH ( 'c', COLOR_START );
				TRANSITION ( STYLE_NAME_START );
			}
			
			//----------------------------------------------------------------//
			case STYLE_ABORT: {
				
				this->mIdx = startIdx;
				TRANSITION ( DONE );
			}
			
			//================================================================//
			// COLOR
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			case COLOR_START: {
				
				colorSize = 0;
				c = this->GetChar ();
				
				TRANSITION_ON_MATCH ( ':', COLOR_BODY );
				
				// reset and try to parse style name instead
				this->mIdx = startIdx;
				TRANSITION ( STYLE_NAME_START );
			}
			
			//----------------------------------------------------------------//
			case COLOR_BODY: {
				
				c = this->GetChar ();

				TRANSITION_ON_MATCH ( '>', COLOR_FINISH );
				
				u8 hex = STLString::hex_to_byte ( c );
				if (( hex != 0xff ) && ( colorSize < COLOR_MAX )) {
					color [ colorSize++ ] = hex;
					TRANSITION ( COLOR_BODY );
				}
				TRANSITION ( STYLE_ABORT );
			}
			
			//----------------------------------------------------------------//
			case COLOR_FINISH: {
				
				this->FinishToken ();
				
				MOAITextStyle* style = this->mStyleCache->AddAnonymousStyle ( this->mCurrentStyle );
				style->mColor = this->PackColor ( color, colorSize );
				this->PushStyle ( style );
				
				TRANSITION ( DONE );
			}
			
			//================================================================//
			// STYLE_NAME
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			case STYLE_NAME_START: {
				
				c = this->GetChar ();
				
				if ( MOAIFont::IsControl ( c ) || MOAIFont::IsWhitespace ( c )) {
					TRANSITION ( STYLE_ABORT );
				}
				
				TRANSITION_ON_MATCH ( '>', STYLE_NAME_FINISH );
				TRANSITION ( STYLE_NAME_START );
			}
			
			//----------------------------------------------------------------//
			case STYLE_NAME_FINISH: {

				this->FinishToken ();

				int namesize = this->mIdx - startIdx - 2;
				assert ( namesize > 0 );

				char* name = ( char* )alloca ( namesize + 1 );
				memcpy ( name, &this->mStr [ startIdx + 1 ], namesize );
				name [ namesize ] = 0;
				
				MOAITextStyle* style = this->mStyleCache->GetStyle ( name );
				this->PushStyle ( style );
				
				TRANSITION ( DONE );
			}
			
			//================================================================//
			// STYLE_POP
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			case STYLE_POP_START: {
				
				c = this->GetChar ();
				
				if ( MOAIFont::IsControl ( c ) || MOAIFont::IsWhitespace ( c )) {
					TRANSITION ( STYLE_ABORT );
				}
				
				TRANSITION_ON_MATCH ( '>', STYLE_POP_FINISH );
				TRANSITION ( STYLE_POP_START );
			}
			
			//----------------------------------------------------------------//
			case STYLE_POP_FINISH: {
				
				this->FinishToken ();
				this->PopStyle ();
				TRANSITION ( DONE );
			}
		}
	}
	
	return ( this->mIdx > startIdx );
}

//----------------------------------------------------------------//
void MOAITextStyleParser::PopStyle () {

	if ( this->mStyleStack.GetTop () > 1 ) {
		this->mStyleStack.Pop ();
		this->mCurrentStyle = this->mStyleStack [ this->mStyleStack.GetTop () - 1 ];
	}
}

//----------------------------------------------------------------//
void MOAITextStyleParser::PushStyle ( MOAITextStyle* style ) {

	assert ( style );
	
	u32 styleID = 0;
	u32 totalStyles = this->mActiveStyles.GetTop ();
	for ( ; styleID < totalStyles; ++styleID ) {
		if ( this->mActiveStyles [ styleID ] == style ) {
			break;
		}
	}
	
	if ( styleID >= totalStyles ) {
		this->mActiveStyles.Push ( style );
	}

	this->mStyleStack.Push ( style );
	this->mCurrentStyle = style;
}

//----------------------------------------------------------------//
void MOAITextStyleParser::UngetChar () {

	this->mIdx = this->mPrev;
}