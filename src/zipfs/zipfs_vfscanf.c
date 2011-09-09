// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zipfs/zipfs.h>
#include <zipfs/ZIPFSString.h>

// poor man's vfscanf
// incomplete and non-ANSI
// gettin' it done for zipfs stdio lookalike api
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
void	buffer_char				( ZIPFSFILE* file, ZIPFSString* buffer );
void	buffer_float			( ZIPFSFILE* file, ZIPFSString* buffer );
void	buffer_hex				( ZIPFSFILE* file, ZIPFSString* buffer );
void	buffer_int				( ZIPFSFILE* file, ZIPFSString* buffer );
int		is_digit				( int c );
int		is_hex_digit			( int c );
int		is_modifier				( int c );
int		is_sign					( int c );
int		is_sign_or_digit		( int c );
int		is_whitespace			( int c );
char	read_char				( ZIPFSString* buffer, ZIPFSFILE* file );
int		read_format_specifier	( const char* format, FormatSpecifier* specifier );

//----------------------------------------------------------------//
void buffer_char ( ZIPFSFILE* file, ZIPFSString* buffer ) {

	int c;

	buffer->mStrLen = 0;

	c = zipfs_getc ( file );
	if ( c != EOF ) {
		ZIPFSString_PushChar ( buffer, ( char )c );
	}
}

//----------------------------------------------------------------//
void buffer_float ( ZIPFSFILE* file, ZIPFSString* buffer ) {

	int backup = 1;
	char c;
	
	buffer->mStrLen = 0;
	
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

	buffer->mStrLen -= backup;
	buffer->mMem [ buffer->mStrLen ] = 0;
	
	if ( c == 0 ) {
		backup--;
	}
	zipfs_fseek ( file, -backup, SEEK_CUR );
}

//----------------------------------------------------------------//
void buffer_hex ( ZIPFSFILE* file, ZIPFSString* buffer ) {

	char c;
	
	buffer->mStrLen = 0;
	
	c = read_char ( buffer, file );
	if ( !is_hex_digit ( c )) goto finish;
	
	if ( c == '0' ) {
	
		c = read_char ( buffer, file );
		if ( !( is_hex_digit ( c ) || ( c == 'x' ) || ( c == 'X' ))) goto finish;
	}
	
	while ( is_hex_digit ( read_char ( buffer, file )));

finish:

	ZIPFSString_PopChar ( buffer );
	zipfs_fseek ( file, -1, SEEK_CUR );
}

//----------------------------------------------------------------//
void buffer_int ( ZIPFSFILE* file, ZIPFSString* buffer ) {

	buffer->mStrLen = 0;

	while ( is_sign_or_digit ( read_char ( buffer, file )));
	
	ZIPFSString_PopChar ( buffer );
	zipfs_fseek ( file, -1, SEEK_CUR );
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
char read_char ( ZIPFSString* buffer, ZIPFSFILE* file ) {

	int result = zipfs_fgetc ( file );
	char c = result == EOF ? 0 : ( char )result;
	
	ZIPFSString_PushChar ( buffer, c );
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
// zipfs_vfscanf
//================================================================//

//----------------------------------------------------------------//
int zipfs_vfscanf ( ZIPFSFILE* file, const char* format, va_list arg ) {

	int result;
	int count = 0;
	ZIPFSString buffer;
	FormatSpecifier specifier;
	char scanBuffer [ SCAN_BUFFER_SIZE ];
	
	ZIPFSString_SetBuffer ( &buffer, scanBuffer, SCAN_BUFFER_SIZE );
	
	while ( read_format_specifier ( format, &specifier ) && !zipfs_feof ( file )) {
		format = &format [ specifier.mFormatLength ];
		result = 0;
		
		if ( specifier.mSpecStr ) {
			
			void* out = specifier.mIgnore ? 0 : va_arg ( arg, void* );
			
			switch ( specifier.mType ) {
				
				case INPUT_TYPE_CHAR: {
					buffer_char ( file, &buffer );
					break;
				}
				case INPUT_TYPE_INT:
				case INPUT_TYPE_UINT:
				case INPUT_TYPE_OCTAL: {
					buffer_int ( file, &buffer );
					break;
				}
				case INPUT_TYPE_FLOAT: {
					buffer_float ( file, &buffer );
					break;
				}
				case INPUT_TYPE_STRING: {
					break;
				}
				case INPUT_TYPE_HEX: {
					buffer_hex ( file, &buffer );
					break;
				}
				default:
					goto finish;
			}
			
			result = out ? sscanf ( buffer.mMem, specifier.mSpecStr, out ) : sscanf ( buffer.mMem, specifier.mSpecStr );
			if ( result == EOF ) goto finish;
		}
		else {
		
			switch ( specifier.mType ) {
				
				case INPUT_TYPE_STRING: {
				
					if ( specifier.mIgnore ) {
						int c = zipfs_getc ( file );
						while ( c && !(( c == EOF ) || is_whitespace ( c ))) {
							c = zipfs_getc ( file );
						}
					}
					else {
						char* out = specifier.mIgnore ? 0 : va_arg ( arg, char* );
						
						int c = zipfs_getc ( file );
						while ( c && !(( c == EOF ) || is_whitespace ( c ))) {
							( *out++ ) = ( char )c;
							c = zipfs_getc ( file );
							result = 1;
						}	
					}
					break;
				}
				case INPUT_TYPE_MATCH: {
					char c;
					size_t i = 0;
					for ( ; i < specifier.mFormatLength; ++i ) {
						
						c = ( char )zipfs_getc ( file );
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

	ZIPFSString_Clear ( &buffer );
	return count;
}
