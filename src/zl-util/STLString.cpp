// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/STLString.h>
#include <algorithm>
#include <cctype>
#include <iostream>

#include <zl-util/ZLBase64Reader.h>
#include <zl-util/ZLBase64Writer.h>
#include <zl-util/ZLByteStream.h>
#include <zl-util/ZLHexReader.h>
#include <zl-util/ZLHexWriter.h>
#include <zl-util/ZLMemStream.h>

//================================================================//
// STLString
//================================================================//

//----------------------------------------------------------------//
void STLString::base_64_decode ( void* buffer, size_t len ) {

	ZLBase64Reader base64;
	zl_decode ( base64, buffer, len );
}

//----------------------------------------------------------------//
// returns an approx. len no smaller than actual decoded size
size_t STLString::base_64_decode_len ( size_t len ) {

	return ZLBase64Reader::GetDecodedLength ( len );
}

//----------------------------------------------------------------//
void STLString::base_64_encode ( const void* buffer, size_t len ) {
	
	ZLBase64Writer base64;
	zl_encode ( base64, buffer, len );
}

//----------------------------------------------------------------//
// returns an approx. len no smaller than actual encoded size
size_t STLString::base_64_encode_len ( size_t len ) {

	return ZLBase64Writer::GetEncodedLength ( len );
}

//----------------------------------------------------------------//
STLString STLString::clip ( size_t first, size_t last ) {

	return this->substr ( first, last - first );
}

//----------------------------------------------------------------//
STLString STLString::clip_to_back ( size_t first ) {

	return this->substr ( first );
}

//----------------------------------------------------------------//
STLString STLString::clip_to_front ( size_t last ) {

	return this->substr ( 0, last );
}

//----------------------------------------------------------------//
void STLString::hex_decode ( void* buffer, size_t len ) {

	ZLHexReader hex;
	zl_decode ( hex, buffer, len );
}

//----------------------------------------------------------------//
// returns an approx. len no smaller than actual decoded size
size_t STLString::hex_decode_len ( size_t len ) {

	return ZLHexReader::GetDecodedLength ( len );
}

//----------------------------------------------------------------//
void STLString::hex_encode ( const void* buffer, size_t len ) {
	
	ZLHexWriter hex;
	zl_encode ( hex, buffer, len );
}

//----------------------------------------------------------------//
// returns an approx. len no smaller than actual encoded size
size_t STLString::hex_encode_len ( size_t len ) {

	return ZLHexWriter::GetEncodedLength ( len );
}

//----------------------------------------------------------------//
u8 STLString::hex_to_byte ( u32 c ) {

	return ZLHexReader::HexToByte ( c );
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
void STLString::zl_decode ( ZLStreamReader& reader, void* buffer, size_t len ) {

	ZLByteStream byteStream;
	
	byteStream.SetBuffer (( void* )this->str (), this->size ());
	byteStream.SetLength ( this->size ());
	
	reader.Open ( byteStream );
	reader.ReadBytes ( buffer, len );
	reader.Close ();
}

//----------------------------------------------------------------//
void STLString::zl_encode ( ZLStreamWriter& writer, const void* buffer, size_t len ) {

	( *this ) = "";
	if ( !len ) return;
	
	ZLMemStream memStream;
	
	writer.Open ( memStream );
	writer.WriteBytes ( buffer, len );
	writer.Close ();
	
	memStream.Seek ( 0, SEEK_SET );
	( *this ) = memStream.ReadString ( memStream.GetLength ());
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
