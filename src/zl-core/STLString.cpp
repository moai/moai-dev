// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-core/STLString.h>
#include <algorithm>
#include <cctype>
#include <iostream>

#include <zl-core/ZLBase64Reader.h>
#include <zl-core/ZLBase64Writer.h>
#include <zl-core/ZLByteStream.h>
#include <zl-core/ZLHexAdapter.h>
#include <zl-core/ZLMemStream.h>
#include <zl-core/ZLZip.h>

#include <zl-core/zl_util.h>

//================================================================//
// STLString
//================================================================//

//----------------------------------------------------------------//
STLString& STLString::assign ( cc8* str ) {

	if ( str ) {
		this->std::string::assign ( str );
	}
	else {
		this->clear ();
	}
	return *this;
}

//----------------------------------------------------------------//
void STLString::base_64_decode ( void* buffer, size_t len ) {

	ZLBase64Reader base64;
	zl_decode ( base64, buffer, len );
}

//----------------------------------------------------------------//
// returns an approx. len no smaller than actual decoded size
size_t STLString::base_64_decode_len () {

	return ZLBase64Reader::EstimateDecodedLength ( this->size ());
}

//----------------------------------------------------------------//
void STLString::base_64_encode ( const void* buffer, size_t len ) {
	
	ZLBase64Writer base64;
	zl_encode ( base64, buffer, len );
}

//----------------------------------------------------------------//
// returns an approx. len no smaller than actual encoded size
size_t STLString::base_64_encode_len () {

	return ZLBase64Writer::EstimateEncodedLength ( this->size ());
}

//----------------------------------------------------------------//
STLString STLString::build ( cc8* format, ... ) {

	va_list args;
	va_start ( args, format );
	STLString result = STLString::build_var ( format, args );
	va_end ( args );

	return result;
}

//----------------------------------------------------------------//
STLString STLString::build_var ( cc8* format, va_list args ) {

	STLString result;
	result.write_var ( format, args );
	return result;
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

	ZLHexAdapter hex;
	zl_decode ( hex, buffer, len );
}

//----------------------------------------------------------------//
// returns an approx. len no smaller than actual decoded size
size_t STLString::hex_decode_len () {

	return ZLHexAdapter::EstimateDecodedLength ( this->size ());
}

//----------------------------------------------------------------//
void STLString::hex_encode ( const void* buffer, size_t len ) {
	
	ZLHexAdapter hex;
	zl_encode ( hex, buffer, len );
}

//----------------------------------------------------------------//
// returns an approx. len no smaller than actual encoded size
size_t STLString::hex_encode_len () {

	return ZLHexAdapter::EstimateEncodedLength ( this->size ());
}

//----------------------------------------------------------------//
u8 STLString::hex_to_byte ( u32 c ) {

	return ZLHexAdapter::HexToByte ( c );
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
size_t STLString::tokenize ( STLArray < STLString > & tokens, const STLString& delimiters ) const {

	STLString::size_type lastPos = this->find_first_not_of ( delimiters, 0 );
	STLString::size_type pos = this->find_first_of ( delimiters, lastPos );

	while (( STLString::npos != pos ) || ( STLString::npos != lastPos )) {
        tokens.push_back ( this->substr ( lastPos, pos - lastPos ));
		lastPos = this->find_first_not_of ( delimiters, pos );
		pos = this->find_first_of ( delimiters, lastPos );
	}
	return tokens.size ();
}

//----------------------------------------------------------------//
size_t STLString::tokenize ( const STLString& str, STLArray < STLString >& tokens, const STLString& delimiters ) {

	return str.tokenize ( tokens, delimiters );
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

	transform ( this->begin (), this->end (), this->begin (), ( char( * )( char ))tolower );
}

//----------------------------------------------------------------//
void STLString::to_upper () {

	transform ( this->begin (), this->end (), this->begin (), ( char( * )( char ))toupper );
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

	char buffer [ 1024 ];
	char* str = zl_vsnprintf_alloc ( buffer, sizeof ( buffer ), format, args );
	
	this->append ( str );
	
	if ( buffer != str ) {
		free ( str );
	}
}

//----------------------------------------------------------------//
void STLString::zip_deflate ( const void* buffer, size_t len ) {

	ZLLeanArray < u8 > zip;
	ZLZip::Deflate ( buffer, len, zip );
	this->base_64_encode ( zip.GetBuffer (), zip.Size ());
}

//----------------------------------------------------------------//
size_t STLString::zip_inflate ( void* buffer, size_t len ) {

	size_t zipLen = this->base_64_decode_len ();
	
	void* zip = malloc ( zipLen );
	this->base_64_decode ( zip, zipLen );
	
	ZLLeanArray < u8 > unzip;
	ZLZip::Inflate ( zip, zipLen, unzip );
	
	free ( zip );
	
	size_t unzipLen = unzip.Size ();
	
	len = unzipLen <= len ? unzipLen : len;
	memcpy ( buffer, unzip.GetBuffer (), len );
	
	return unzipLen;
}

//----------------------------------------------------------------//
void STLString::zl_decode ( ZLStreamAdapter& reader, void* buffer, size_t len ) {

	ZLByteStream byteStream;
	
	byteStream.SetBuffer (( void* )this->str (), this->size ());
	byteStream.SetLength ( this->size ());
	
	reader.Open ( &byteStream );
	reader.ReadBytes ( buffer, len );
	reader.Close ();
}

//----------------------------------------------------------------//
void STLString::zl_encode ( ZLStreamAdapter& writer, const void* buffer, size_t len ) {

	( *this ) = "";
	if ( !len ) return;
	
	ZLMemStream memStream;
	
	writer.Open ( &memStream );
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
