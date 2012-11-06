// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIFontReader.h>
#include <moaicore/MOAIGlyphSet.h>
#include <moaicore/MOAIStaticGlyphCache.h>
#include <moaicore/MOAITexture.h>

#if defined( _WIN32 )
	#define strcasecmp(str1, str2) ( stricmp ( str1,str2 ))
#endif

#define WIDE_ID_BIT			0x80000000
#define WIDE_ID_MASK		0x7fffffff
#define INVALID_ID			0xffffffff
#define INVALID_BYTE_ID		0xff

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
static char* parseKeyVal ( char *p, char **key, char **val, bool *endl ) {

	while ( *p && !isalnum ( *p )) {
		p++;
	}

	*key = p;
	while ( *p && isalnum ( *p )) {
		p++;
	}

	if( *p == '=' ) {
	
		*p = '\0'; // Terminate the key string
		p++;
		if( *p == '"' ) {
			// Parse a string literal (with escaped '"' chars)
			p++;
			*val = p;
			if ( p [ 0 ] == '"' && p [ 1 ] == '"' ) {
				// Special case """ because some tools incorrectly export this, as in: letter="""
				p++;
			}
			else {
				while ( *p && *p != '\n' && !( *p == '"' && p [ -1 ] != '\\' )) {
					p++;
				}
			}
			if ( *p == '"' ) {
				*p++ = '\0';
			}
			*endl = ( *p == '\r' || *p == '\n' || *p == 0 );
		}
		else {
			// Parse a simple non-white block
			*val = p;
			while ( *p && !isspace ( *p )) {
				p++;
			}
			*endl = ( *p == '\r' || *p == '\n' || *p == 0 );
			if ( *p ) {
				*p++ = '\0';
			}
			else {
				*p = '\0';
			}
		}
	}
	else {
		*endl = ( *p == '\r' || *p == '\n' || *p == 0 );
		if ( *p ) {
			*p++ = '\0';
		}
		else {
			*p = '\0';
		}
		*val = 0;
	}
	return p;
}

//================================================================//
// MOAIFont_bmfont
//================================================================//

//----------------------------------------------------------------//
void MOAIFont::InitWithBMFont ( cc8* filename ) {

	USFileStream stream;
	if ( !stream.OpenRead ( filename )) return;

	STLString absFilePath = USFileSys::GetAbsoluteFilePath ( filename );
	STLString absDirPath = USFileSys::TruncateFilename ( absFilePath );

	u32 len = stream.GetLength ();
	char* buf = ( char* )malloc ( len + 1 );
	stream.ReadBytes ( buf, len );
	buf [ len ] = '\0';
	stream.Close ();

	char* p = buf;
	char* endp = p + len;

	MOAIGlyphSet* glyphSet = 0;
	MOAIStaticGlyphCache* glyphCache = new MOAIStaticGlyphCache ();

	this->mCache.Set ( *this, glyphCache );
	this->mReader.Set ( *this, 0 );

	p = buf;
	while ( p < endp ) {
	
		// Parse each line.
		bool endl;
		char* key;
		char* val;

		p = parseKeyVal ( p, &key, &val, &endl );

		if ( strcmp ( key, "info" ) == 0 ) {
		
			float size = 0.0f;
			
			//info face="Cambria" size=64 bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=2,2
			do {
				p = parseKeyVal ( p, &key, &val, &endl );
				if ( strcasecmp ( key, "size" ) == 0 ) { size = ( float )atof ( val ); }
			} while ( !endl );
			
			if ( size > 0.0f ) {
				glyphSet = &this->AffirmGlyphSet ( size );
				assert ( glyphSet );
			}
		}
		else if ( strcmp ( key, "common" ) == 0 ) {
			
			float lineSpacing = 0.0f;
			float base = 0.0f;
			u32 pages = 0;
			
			//common lineHeight=75 base=61 scaleW=512 scaleH=512 pages=1 packed=0
			do {
				p = parseKeyVal ( p, &key, &val, &endl );
				if ( strcasecmp ( key, "lineHeight" ) == 0 ) { lineSpacing = ( float )atof ( val ); }
				else if ( strcasecmp ( key, "pages" ) == 0 ) { pages = ( u32 )atoi ( val ); }
				else if ( strcasecmp ( key, "base" ) == 0 ) { base = ( float )atof ( val ); }
			} while ( !endl );
			
			glyphSet->SetHeight( lineSpacing );
			glyphSet->SetAscent( base );
			glyphCache->ReserveTextures ( pages );
		}
		else if ( strcmp ( key, "page" ) == 0 ) {
			
			STLString texturename;
			u32 id = 0;
		
			//page id=0 file="Blah.png"
			do {
				p = parseKeyVal ( p, &key, &val, &endl );
				if( strcmp ( key, "id" ) == 0 ) { id = ( u32 )atoi ( val ); }
				else if ( strcmp ( key, "file" ) == 0 ) {
					texturename = absDirPath;
					texturename.append ( val );
				}
			} while ( !endl );
			
			MOAITexture* texture = new MOAITexture ();
			glyphCache->SetTexture ( id, texture );
			texture->Init ( texturename, MOAITexture::DEFAULT_TRANSFORM );
		}
		else if ( strcmp ( key, "chars" ) == 0 ) {
			//chars count=95
			do {
				p = parseKeyVal ( p, &key, &val, &endl );
			} while ( !endl );
		}
		else if ( strcmp ( key, "char" ) == 0 ) {
			
			//char id=47 x=2 y=2 width=32 height=63 xoffset=1 yoffset=15 xadvance=31 page=0 chnl=0 letter="/"
			u32 c = 0;
			u32 x = 0;
			u32 y = 0;
			float width = 0.0f;
			float height = 0.0f;
			float xoff = 0.0f;
			float yoff = 0.0f;
			float xadv = 0.0f;
			u32 page = 0;
			
			do {
				p = parseKeyVal ( p, &key, &val, &endl );
				if ( strcasecmp ( key, "id" ) == 0 ) { c = ( u32 )atoi ( val ); }
				else if ( strcasecmp ( key, "x" ) == 0 ) { x = ( u32 )atoi ( val ); }
				else if ( strcasecmp ( key, "y" ) == 0 ) { y = ( u32 )atoi ( val ); }
				else if ( strcasecmp ( key, "width" ) == 0 ) { width = ( float )atof ( val ); }
				else if ( strcasecmp ( key, "height" ) == 0 ) { height = ( float )atof ( val ); }
				else if ( strcasecmp ( key, "xoffset" ) == 0 ) { xoff = ( float )atof ( val ); }
				else if ( strcasecmp ( key, "yoffset" ) == 0 ) { yoff = ( float )atof ( val ); }
				else if ( strcasecmp ( key, "xadvance" ) == 0 ) { xadv = ( float )atof ( val ); }
				else if ( strcasecmp ( key, "page" ) == 0 ) { page = ( u32 )atoi ( val ); }

			} while( !endl );
			
			assert ( glyphSet );
			MOAIGlyph& glyph = glyphSet->EditGlyph ( c );
			
			glyph.mSrcX = x;
			glyph.mSrcY = y;
			glyph.mPageID = page;
			glyph.mWidth = width;
			glyph.mHeight = height;
			glyph.mAdvanceX = xadv;
			glyph.mBearingX = xoff;
			glyph.mBearingY = glyphSet->GetAscent() - yoff;
			
		}
		else if ( strcmp ( key, "kernings" ) == 0 ) {
			//kernings count=560
			do {
				p = parseKeyVal ( p, &key, &val, &endl );
			} while ( !endl );
		}
		else if ( strcmp ( key, "kerning" ) == 0 ) {
			
			//kerning first=47 second=65 amount=-1
			u32 first = 0;
			u32 second = 0;
			float amount = 0.0f;

			do {
				p = parseKeyVal ( p, &key, &val, &endl );
				if( strcasecmp ( key, "first" ) == 0 ) { first = ( u32 )atoi ( val ); }
				else if ( strcasecmp ( key, "second" ) == 0 ) { second = ( u32 )atoi ( val ); }
				else if ( strcasecmp ( key, "amount" ) == 0 ) { amount =( float )atof ( val ); }
			} while( !endl );
			
			if ( first && second && ( amount != 0.0f )) {
			
				assert ( glyphSet );
				MOAIGlyph& glyph = glyphSet->EditGlyph ( first );
				
				u32 i = glyph.mKernTable.Size();
				glyph.mKernTable.Grow ( i + 1 );
				glyph.mKernTable [ i ].mName = second;
				glyph.mKernTable [ i ].mX = amount;
				glyph.mKernTable [ i ].mY = 0;
			}
		}
	}
}
