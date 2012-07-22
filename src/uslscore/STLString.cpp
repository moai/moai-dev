// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/STLString.h>
#include <algorithm>
#include <cctype>
#include <iostream>

#include <uslscore/USBase64Reader.h>
#include <uslscore/USBase64Writer.h>
#include <uslscore/USByteStream.h>
#include <uslscore/USMemStream.h>

//================================================================//
// STLString
//================================================================//

//----------------------------------------------------------------//
void STLString::base_64_decode ( void* buffer, u32 len ) {

	USByteStream byteStream;
	USBase64Reader base64;
	
	byteStream.SetBuffer (( void* )this->str (), this->size ());
	byteStream.SetLength ( this->size ());
	
	base64.Open ( &byteStream );
	base64.ReadBytes ( buffer, len );
	base64.Close ();
}

//----------------------------------------------------------------//
void STLString::base_64_encode ( const void* buffer, u32 len ) {
	
	( *this ) = "";
	if ( !len ) return;
	
	USMemStream memStream;
	USBase64Writer base64;
	
	base64.Open ( &memStream );
	base64.WriteBytes ( buffer, len );
	base64.Close ();
	
	memStream.Seek ( 0, SEEK_SET );
	( *this ) = memStream.ReadString ( memStream.GetLength ());
}

//----------------------------------------------------------------//
STLString STLString::clip ( u32 first, u32 last ) {

	return this->substr ( first, last - first );
}

//----------------------------------------------------------------//
STLString STLString::clip_to_back ( u32 first ) {

	return this->substr ( first );
}

//----------------------------------------------------------------//
STLString STLString::clip_to_front ( u32 last ) {

	return this->substr ( 0, last );
}

//----------------------------------------------------------------//
void STLString::hex_encode ( const void* buffer, u32 len ) {

	if ( !len ) {
		( *this ) = "";
		return;
	}

	u8* digits = ( u8* )buffer;
	
	char* hexStr = ( char* )alloca (( len * 2 ) + 1 );
	char* hexPtr = hexStr;
	for ( size_t i = 0; i < len; ++i ) {
		hexPtr += sprintf ( hexPtr, "%02x", digits [ i ]);
	}

	( *this ) = hexStr;
}

//----------------------------------------------------------------//
u8 STLString::hex_to_byte ( u32 c ) {

	if (( c >= '0' ) && ( c <= '9' )) return ( u8 )( c - '0' );
	if (( c >= 'a' ) && ( c <= 'f' )) return ( u8 )( c + 10 - 'a' );
	if (( c >= 'A' ) && ( c <= 'F' )) return ( u8 )( c + 10 - 'A' );

	return 0xff;
}

//----------------------------------------------------------------//
void STLString::replace_char ( cc8 match, cc8 sub ) {

	for ( u32 i = 0; this->peek ( i ); ++i ) {
		if ( this->peek ( i ) == match ) {
			this->at ( i ) = sub;
		}
	}
}

//----------------------------------------------------------------//
void STLString::tokenize ( STLArray < STLString > & tokens, const STLString& delimiters ) {

	STLString::size_type lastPos = this->find_first_not_of ( delimiters, 0 );
	STLString::size_type pos = this->find_first_of ( delimiters, lastPos );

	while (( STLString::npos != pos ) || ( STLString::npos != lastPos )) {
        tokens.push_back ( this->substr ( lastPos, pos - lastPos ));
		lastPos = this->find_first_not_of ( delimiters, pos );
		pos = this->find_first_of ( delimiters, lastPos );
	}
}

//----------------------------------------------------------------//
double STLString::to_double () {

	return atof ( this->str ());
}

//----------------------------------------------------------------//
float STLString::to_float () {

	return ( float )atof ( this->str ());
}

//----------------------------------------------------------------//
int STLString::to_int () {

	return atoi ( this->str ());
}

//----------------------------------------------------------------//
void STLString::to_lower () {

	transform ( this->begin (), this->end (),this->begin (), ( int( * )( int ))tolower );
}

//----------------------------------------------------------------//
void STLString::to_upper () {

	transform ( this->begin (), this->end (),this->begin (), ( int( * )( int ))toupper );
}

//----------------------------------------------------------------//
void STLString::write ( cc8* format, ... ) {

	va_list args;
	va_start ( args, format );
	
	this->write_var ( format, args );
	
	va_end ( args );
}

//----------------------------------------------------------------//
void STLString::write_var ( cc8* format, va_list args ) {

	static const u32 BUFFER_SIZE = 1024;
	char stackBuffer [ BUFFER_SIZE ];
	char* buffer = stackBuffer;
	int buffSize = BUFFER_SIZE;
	
	int result;
	
	for ( ;; ) {
	
		result = vsnprintf ( buffer, buffSize, format, args );

		// thanks to http://perfec.to/vsnprintf/ for a discussion of vsnprintf portability issues
		if (( result == buffSize ) || ( result == -1 ) || ( result == buffSize - 1 ))  {
			buffSize = buffSize << 1;
		}
		else if ( result > buffSize ) {
			buffSize = result;
		}
		else {
			break;
		}
		
		if ( buffer == stackBuffer ) {
			buffer = 0;
		}
		
		if ( buffer ) {
			buffer = ( char* )realloc ( buffer, buffSize );
		}
		else {
			buffer = ( char* )malloc ( buffSize );
		}
	}
	
	this->append ( buffer );
	
	if ( buffer != stackBuffer ) {
		free ( buffer );
	}
}

//----------------------------------------------------------------//

STLString::STLString ( int value, int radix ) {

	const char *format;

	switch ( radix ) {

		case 8:
			format = "%o";
			break;
		case 10:
			format = "%i";
			break;
		case 16:
			format = "%08X";
			break;
		default:
			format = "%i";
	}

	write ( format, value );
}

//----------------------------------------------------------------//
STLString::STLString ( double value ) {

	write ( "%f", value );
}
