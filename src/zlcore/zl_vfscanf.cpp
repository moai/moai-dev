// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zlcore/zlcore.h>

using namespace std;

// poor man's vfscanf
// incomplete and non-ANSI
// gettin' it done for zlcore stdio lookalike api
// needs improvement as we go

#define SCAN_BUFFER_SIZE 1024

//================================================================//
// ScanArg
//================================================================//
typedef struct FormatSpecifier {

	int		mIgnore;
	int		mWidth;
	int		mType;
	
	size_t mFormatLength;
	char const*	mSpecStr;
	char const*	mMatch;

} FormatSpecifier;

enum {
	INPUT_TYPE_UNKNOWN,
	INPUT_TYPE_CHAR,
	INPUT_TYPE_INT,
	INPUT_TYPE_FLOAT,
	INPUT_TYPE_OCTAL,
	INPUT_TYPE_STRING,
	INPUT_TYPE_UINT,
	INPUT_TYPE_HEX,
	INPUT_TYPE_MATCH,
};

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
void	buffer_char				( string& buffer, ZLFILE* file );
void	buffer_float			( string& buffer, ZLFILE* file );
void	buffer_hex				( string& buffer, ZLFILE* file );
void	buffer_int				( string& buffer, ZLFILE* file );
int		is_digit				( int c );
int		is_hex_digit			( int c );
int		is_modifier				( int c );
int		is_sign					( int c );
int		is_sign_or_digit		( int c );
int		is_whitespace			( int c );
char	read_char				( string& buffer, ZLFILE* file );
int		read_format_specifier	( const char* format, FormatSpecifier* specifier );

//----------------------------------------------------------------//
void buffer_char ( string& buffer, ZLFILE* file ) {

	int c;

	c = zl_getc ( file );
	if ( c != EOF ) {
		buffer.push_back (( char )c );
	}
}

//----------------------------------------------------------------//
void buffer_float ( string& buffer, ZLFILE* file ) {

	int backup = 1;
	char c;
	
	c = read_char ( buffer, file );
	
	// sign and digits
	if ( is_sign ( c )) {
		c = read_char ( buffer, file );
		backup++;
	}

	if ( is_digit ( c )) {
		while ( is_digit ( c = read_char ( buffer, file )));
		backup = 1;
	}
	else {
		goto finish;
	}
	
	// decimal point and digits
	if ( c == '.' ) {
		c = read_char ( buffer, file );
		backup++;
		
		if ( is_digit ( c )) {
			backup = 1;
			while ( is_digit ( c = read_char ( buffer, file )));
		}
		else {
			goto finish;
		}
	}
	
	// exponent
	if ( !( c == 'e' ) || ( c == 'E' )) goto finish;
	backup++;

	// sign or first digit
	c = read_char ( buffer, file );
	
	// sign and digits
	if ( is_sign ( c )) {
		backup++;
		c = read_char ( buffer, file );
	}

	if ( is_digit ( c )) {
		while ( is_digit ( c = read_char ( buffer, file )));
		backup = 1;
	}	

finish:

	buffer [ buffer.length () - backup ] = 0;
	
	if ( c == 0 ) {
		backup--;
	}
	zl_fseek ( file, -backup, SEEK_CUR );
}

//----------------------------------------------------------------//
void buffer_hex ( string& buffer, ZLFILE* file ) {

	char c;
	
	c = read_char ( buffer, file );
	if ( !is_hex_digit ( c )) goto finish;
	
	if ( c == '0' ) {
	
		c = read_char ( buffer, file );
		if ( !( is_hex_digit ( c ) || ( c == 'x' ) || ( c == 'X' ))) goto finish;
	}
	
	while ( is_hex_digit ( read_char ( buffer, file )));

finish:

	buffer [ buffer.length () - 1 ] = 0;
	zl_fseek ( file, -1, SEEK_CUR );
}

//----------------------------------------------------------------//
void buffer_int ( string& buffer, ZLFILE* file ) {

	while ( is_sign_or_digit ( read_char ( buffer, file )));
	
	buffer [ buffer.length () - 1 ] = 0;
	zl_fseek ( file, -1, SEEK_CUR );
}

//----------------------------------------------------------------//
int is_digit ( int c ) {

	return (( c >= '0' ) && ( c <= '9' )) ? 1 : 0;
}

//----------------------------------------------------------------//
int is_hex_digit ( int c ) {

	return (( c >= '0' ) && ( c <= '9' )) || (( c >= 'a' ) && ( c <= 'f' )) || (( c >= 'A' ) && ( c <= 'F' )) ? 1 : 0;
}

//----------------------------------------------------------------//
int is_modifier ( int c ) {

	return (( c == 'h' ) || ( c == 'l' ) || ( c == 'L' ));
}

//----------------------------------------------------------------//
int is_sign ( int c ) {

	return (( c == '+' ) || ( c == '-' ));
}

//----------------------------------------------------------------//
int is_sign_or_digit ( int c ) {

	return is_sign ( c ) || is_digit ( c );
}

//----------------------------------------------------------------//
int is_whitespace ( int c ) {

	return (( c == ' ' ) || ( c == '\n' ) || ( c == '\t' )) ? 1 : 0;
}

//----------------------------------------------------------------//
char read_char ( string& buffer, ZLFILE* file ) {

	int result = zl_fgetc ( file );
	char c = result == EOF ? 0 : ( char )result;
	
	buffer.push_back ( c );
	return c;
}

//----------------------------------------------------------------//
int read_format_specifier ( const char* format, FormatSpecifier* specifier ) {

	size_t i = 0;

	if ( !format [ i ]) return 0;
	
	specifier->mIgnore = 0;
	specifier->mWidth = -1;
	specifier->mType = INPUT_TYPE_MATCH;
	specifier->mSpecStr = 0;
	specifier->mFormatLength = 0;
	
	// check for a specifier
	if ( format [ i ] == '%' ) {
		
		specifier->mSpecStr = &format [ i++ ];
		
		// ignore flag
		if ( format [ i ] == '*' ) {
			specifier->mIgnore = 1;
			i++;
		}
		
		// width
		if ( is_digit ( format [ i ])) {
			specifier->mWidth = 0;
			for ( ; is_digit ( format [ i ]); ++i ) {
				specifier->mWidth *= 10;
				specifier->mWidth += format [ i ] - '0';
			}
		}
		
		// type
		switch ( format [ i++ ]) {
			case 'c':
				specifier->mType = INPUT_TYPE_CHAR;
				break;
			case 'd':
				specifier->mType = INPUT_TYPE_INT;
				break;
			case 'e':
			case 'E':
			case 'f':
			case 'g':
			case 'G':
				specifier->mType = INPUT_TYPE_FLOAT;
				break;
			case 'o':
				specifier->mType = INPUT_TYPE_OCTAL;
				break;
			case 's':
				specifier->mType = INPUT_TYPE_STRING;
				specifier->mSpecStr = 0;
				break;
			case 'u':
				specifier->mType = INPUT_TYPE_UINT;
				break;
			case 'x':
			case 'X':
				specifier->mType = INPUT_TYPE_HEX;
				break;
		}

		if ( specifier->mType != INPUT_TYPE_MATCH ) {
			specifier->mFormatLength = i;
			return 1;
		}
	}
	
	// check to see how many characters to match
	specifier->mSpecStr = 0;
	specifier->mMatch = format;
	for ( i = 0; format [ i ] && ( format [ i ] != '%' ); i++ );
	specifier->mFormatLength = i;
	
	return 1;
}

//================================================================//
// zl_vfscanf
//================================================================//

#ifndef MOAI_COMPILER_MSVC
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif

//----------------------------------------------------------------//
int zl_vfscanf ( ZLFILE* file, const char* format, va_list arg ) {

	int result;
	int count = 0;
	FormatSpecifier specifier;
	
	string buffer;	
	buffer.reserve ( SCAN_BUFFER_SIZE );
	
	while ( read_format_specifier ( format, &specifier ) && !zl_feof ( file )) {
		format = &format [ specifier.mFormatLength ];
		result = 0;
		
		if ( specifier.mSpecStr ) {
			
			void* out = specifier.mIgnore ? 0 : va_arg ( arg, void* );
			
			switch ( specifier.mType ) {
				
				case INPUT_TYPE_CHAR: {
					buffer_char ( buffer, file );
					break;
				}
				case INPUT_TYPE_INT:
				case INPUT_TYPE_UINT:
				case INPUT_TYPE_OCTAL: {
					buffer_int ( buffer, file );
					break;
				}
				case INPUT_TYPE_FLOAT: {
					buffer_float ( buffer, file );
					break;
				}
				case INPUT_TYPE_STRING: {
					break;
				}
				case INPUT_TYPE_HEX: {
					buffer_hex ( buffer, file );
					break;
				}
				default:
					goto finish;
			}
			
			result = out ? sscanf ( buffer.c_str (), specifier.mSpecStr, out ) : sscanf ( buffer.c_str (), specifier.mSpecStr );
			
			if ( result == EOF ) goto finish;
		}
		else {
		
			switch ( specifier.mType ) {
				
				case INPUT_TYPE_STRING: {
				
					if ( specifier.mIgnore ) {
						int c = zl_getc ( file );
						while ( c && !(( c == EOF ) || is_whitespace ( c ))) {
							c = zl_getc ( file );
						}
					}
					else {
						char* out = specifier.mIgnore ? 0 : va_arg ( arg, char* );
						
						int c = zl_getc ( file );
						while ( c && !(( c == EOF ) || is_whitespace ( c ))) {
							( *out++ ) = ( char )c;
							c = zl_getc ( file );
							result = 1;
						}	
					}
					break;
				}
				case INPUT_TYPE_MATCH: {
					char c;
					size_t i = 0;
					for ( ; i < specifier.mFormatLength; ++i ) {
						
						c = ( char )zl_getc ( file );
						if ( c != specifier.mMatch [ i ]) goto finish;
					}
					break;
				}
			}	
		}
		
		if ( result ) {
			++count;
		}
	}
	
finish:

	return count;
}

#ifndef MOAI_COMPILER_MSVC
	#pragma GCC diagnostic pop
#endif
