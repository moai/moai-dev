// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAITextLayout.h>
#include <moaicore/MOAITextStyle.h>
#include <moaicore/MOAITextStyler.h>
#include <moaicore/MOAITextStyleSet.h>

//================================================================//
// MOAITextStyler
//================================================================//

#define TRANSITION(next)					\
	state = next;							\
	break;

#define TRANSITION_ON_MATCH(match,next)		\
	if ( c == match ) {						\
		TRANSITION ( next )					\
	}

//----------------------------------------------------------------//
void MOAITextStyler::FinishToken () {
	
	int size = this->mTokenTextSize + this->mTokenWhitespaceSize;
				
	if ( size ) {
		assert ( this->mCurrentStyle );
		this->mLayout->PushToken ( this->mStr, this->mTokenBase, size, 0, *this->mCurrentStyle );
	}
	
	this->mTokenBase = this->mIdx;
	this->mTokenTextSize = 0;
	this->mTokenWhitespaceSize = 0;
}

//----------------------------------------------------------------//
u32 MOAITextStyler::GetChar () {

	this->mPrev = this->mIdx;
	if ( this->mStr [ this->mIdx ]) {
		return u8_nextchar ( this->mStr, &this->mIdx );
	}
	++this->mIdx;
	return 0;
}

//----------------------------------------------------------------//
u8 MOAITextStyler::HexToByte ( u32 c ) {

	if (( c >= '0' ) && ( c <= '9' )) return ( u8 )( c - '0' );
	if (( c >= 'a' ) && ( c <= 'f' )) return ( u8 )( c + 10 - 'a' );
	if (( c >= 'A' ) && ( c <= 'F' )) return ( u8 )( c + 10 - 'A' );

	return 0xff;
}

//----------------------------------------------------------------//
bool MOAITextStyler::IsWhitespace ( u32 c ) {

	if ( !c ) return true;
	if ( c == ' ' ) return true;
	if ( c == '\t' ) return true;
	if ( c == '\n' ) return true;
	
	return false;
}

//----------------------------------------------------------------//
MOAITextStyler::MOAITextStyler () :
	mIdx ( 0 ),
	mStr ( 0 ) {
}

//----------------------------------------------------------------//
MOAITextStyler::~MOAITextStyler () {
}

//----------------------------------------------------------------//
u32 MOAITextStyler::PackColor ( const u8* color, u32 colorSize ) {

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
void MOAITextStyler::Parse () {
	
	u32 c = 0;

	u32 state = TOKEN_WHITESPACE;
	while ( state != DONE ) {
		
		switch ( state ) {
			
			//================================================================//
			// TOKEN
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			// check to see if we've encountered a style escape
			case TOKEN_WHITESPACE: {
				
				if ( this->ParseStyle ()) {
					TRANSITION ( TOKEN_WHITESPACE );
				}
				
				c = this->GetChar ();

				if ( c == 0 ) {
					this->FinishToken ();
					TRANSITION ( DONE );
				}
				
				if ( !this->IsWhitespace ( c )) {
					this->UngetChar ();
					this->FinishToken ();
					TRANSITION ( TOKEN_TEXT );
				}
				
				this->mTokenWhitespaceSize++;
				TRANSITION ( TOKEN_WHITESPACE );
			}
			
			//----------------------------------------------------------------//
			case TOKEN_TEXT: {
				
				if ( this->ParseStyle ()) {
					TRANSITION ( TOKEN_TEXT );
				}
				
				c = this->GetChar ();

				if ( c == 0 ) {
					this->FinishToken ();
					TRANSITION ( DONE );
				}
				
				if ( this->IsWhitespace ( c )) {
					this->UngetChar ();
					TRANSITION ( TOKEN_WHITESPACE );
				}
				
				this->mTokenTextSize++;
				TRANSITION ( TOKEN_TEXT );
			}
		}
	}
}

//----------------------------------------------------------------//
bool MOAITextStyler::ParseStyle () {

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
				
				u8 hex = HexToByte ( c );
				if (( hex != 0xff ) && ( colorSize < COLOR_MAX )) {
					color [ colorSize++ ] = hex;
					TRANSITION ( COLOR_BODY );
				}
				TRANSITION ( STYLE_ABORT );
			}
			
			//----------------------------------------------------------------//
			case COLOR_FINISH: {
				
				//u32 color = this->PackColor ( color, colorSize );
				// TODO: push style
				this->FinishToken ();
				printf ( "finish color\n" );
				TRANSITION ( DONE );
			}
			
			//================================================================//
			// STYLE_NAME
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			case STYLE_NAME_START: {
				
				c = this->GetChar ();
				
				if ( this->IsWhitespace ( c )) {
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
				
				printf ( "found style: %s\n", name );
				
				MOAITextStyle* style = this->mStyleSet->FindStyle ( name );
				this->PushStyle ( style );
				
				TRANSITION ( DONE );
			}
			
			//================================================================//
			// STYLE_POP
			//----------------------------------------------------------------//
			
			//----------------------------------------------------------------//
			case STYLE_POP_START: {
				
				c = this->GetChar ();
				
				if ( this->IsWhitespace ( c )) {
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
void MOAITextStyler::PopStyle () {

	if ( this->mStyleStackTop ) {
		this->mStyleStackTop--;
		
		if ( this->mStyleStackTop && ( this->mStyleStackTop < STYLE_STACK_SIZE )) {
			this->mCurrentStyle = this->mStyleStack [ this->mStyleStackTop - 1 ];
		}
	}	
}

//----------------------------------------------------------------//
void MOAITextStyler::PushStyle ( MOAITextStyle* style ) {

	if ( !style ) {
		style = this->mCurrentStyle;
	}

	if ( this->mStyleStackTop < STYLE_STACK_SIZE ) {
		this->mStyleStack [ this->mStyleStackTop ] = style;
		this->mCurrentStyle = style;
	}
	this->mStyleStackTop++;
}

//----------------------------------------------------------------//
void MOAITextStyler::Style ( MOAITextLayout& layout, MOAITextStyleSet& styleSet, cc8* str ) {
	
	this->mStyleStack = ( MOAITextStyle** )alloca ( STYLE_STACK_SIZE * sizeof ( MOAITextStyle* ));
	this->mStyleStackTop = 0;
	this->mCurrentStyle = 0;
	
	this->PushStyle ( styleSet.GetDefault ());
	if ( !this->mCurrentStyle ) return;
	
	this->mIdx = 0;
	this->mPrev = 0;
	this->mLayout = &layout;
	this->mStr = str;
	this->mStyleSet = &styleSet;
	
	this->mTokenBase = 0;
	this->mTokenTextSize = 0;
	this->mTokenWhitespaceSize = 0;
	
	this->Parse ();
	
	layout.LoadGlyphMetrics ();
	layout.SizeTokens ();
}

//----------------------------------------------------------------//
void MOAITextStyler::UngetChar () {

	this->mIdx = this->mPrev;
}