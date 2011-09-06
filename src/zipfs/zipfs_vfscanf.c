// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zipfs/zipfs.h>
#include <zipfs/ZIPFSString.h>

#define SCAN_BUFFER_SIZE 1024

//================================================================//
// ScanArg
//================================================================//
typedef struct FormatSpecifier {

	int		mIgnore;
	int		mWidth;
	int		mModifier;
	int		mType;
	
	size_t	mFormatLength;
	
} FormatSpecifier;

enum {
	MODIFIER_NONE,
	MODIFIER_SHORT_INT,
	MODIFIER_LONG_INT,
	MODIFIER_LONG_DOUBLE,
};

enum {
	INPUT_TYPE_UNKNOWN,
	INPUT_TYPE_CHAR,
	INPUT_TYPE_INT,
	INPUT_TYPE_FLOAT,
	INPUT_TYPE_OCTAL,
	INPUT_TYPE_STRING,
	INPUT_TYPE_UINT,
	INPUT_TYPE_HEX,
};

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int is_digit ( int c ) {

	return (( c >= '0' ) && ( c <= '9' )) ? 1 : 0;
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
int is_whitespace ( int c ) {

	return (( c == ' ' ) || ( c == '\n' ) || ( c == '\t' )) ? 1 : 0;
}

//----------------------------------------------------------------//
char read_char ( ZIPFSString* buffer, ZIPFSFILE* file ) {

	char c = ( char )zipfs_fgetc ( file );
	ZIPFSString_PushChar ( buffer, c );
	return c;
}

//----------------------------------------------------------------//
int read_format_specifier ( const char* format, FormatSpecifier* specifier ) {

	int c;
	size_t i = 0;

	for ( ; is_whitespace ( format [ i ]); i++ );
	if ( format [ i++ ] != '%' ) return 0;
	
	specifier->mIgnore = 0;
	specifier->mWidth = -1;
	specifier->mModifier = MODIFIER_NONE;
	specifier->mType = INPUT_TYPE_UNKNOWN;
	specifier->mFormatLength = 0;
	
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
	
	// modifier
	if ( is_modifier ( format [ i ])) {
		
		c = format [ i++ ];
		
		if ( c == 'h' ) {
			specifier->mModifier = MODIFIER_SHORT_INT;
		}
		else if ( c == 'l' ) {
			specifier->mModifier = MODIFIER_LONG_INT;
		}
		else {
			specifier->mModifier = MODIFIER_LONG_DOUBLE;
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
			break;
		case 'u':
			specifier->mType = INPUT_TYPE_UINT;
			break;
		case 'x':
		case 'X':
			specifier->mType = INPUT_TYPE_HEX;
			break;
	}

	specifier->mFormatLength = i;
	return 1;
}

//----------------------------------------------------------------//
int scan_char ( ZIPFSFILE* file, va_list arg ) {

	char* out = va_arg ( arg, char* );
	( *out ) = ( char )zipfs_fgetc ( file );
	return 0;
}

//----------------------------------------------------------------//
int scan_float ( ZIPFSFILE* file, va_list arg ) {

	char c;
	int result = -1;
	ZIPFSString buffer;
	char scanBuffer [ SCAN_BUFFER_SIZE ];
	float* out = va_arg ( arg, float* );
	
	ZIPFSString_SetBuffer ( &buffer, scanBuffer, SCAN_BUFFER_SIZE );
	
	// leading whitespace
	while ( is_whitespace ( c = ( char )zipfs_getc ( file )));
	
	// sign or first digit
	if ( !( is_sign ( c ) || is_digit ( c ))) goto finish;
	
	// integer digits
	while ( is_digit ( c = read_char ( &buffer, file )));
	
	if ( c != '.' ) goto done;

	// decimal digits
	while ( is_digit ( c = read_char ( &buffer, file )));
	
	// exponent
	if ( !( c == 'e' ) || ( c == 'E' )) goto done;

	// sign or first digit
	c = read_char ( &buffer, file );
	if ( !( is_sign ( c ) || is_digit ( c ))) goto finish;

	// exponent digits
	while ( is_digit ( c = read_char ( &buffer, file )));

done:

	result = 0;
	ZIPFSString_PopChar ( &buffer );

	*out = ( float )atof ( buffer.mMem );

finish:

	ZIPFSString_Clear ( &buffer );
	zipfs_ungetc ( c, file );
	return result;
}

//----------------------------------------------------------------//
int scan_hex ( ZIPFSFILE* file, va_list arg ) {

	return -1;
}

//----------------------------------------------------------------//
int scan_int ( ZIPFSFILE* file, va_list arg ) {

	return -1;
}

//----------------------------------------------------------------//
int scan_octal ( ZIPFSFILE* file, va_list arg ) {

	return -1;
}

//----------------------------------------------------------------//
int scan_string ( ZIPFSFILE* file, va_list arg ) {

	return -1;
}

//----------------------------------------------------------------//
int scan_unsigned_int ( ZIPFSFILE* file, va_list arg ) {

	return -1;
}

//================================================================//
// zipfs_vfscanf
//================================================================//

//----------------------------------------------------------------//
int zipfs_vfscanf ( ZIPFSFILE* file, const char* format, va_list arg ) {

	size_t i = 0;
	int result;
	FormatSpecifier specifier;
	
	while ( read_format_specifier ( &format [ i ], &specifier )) {
		format = &format [ specifier.mFormatLength ];
		
		switch ( specifier.mType ) {
		
			case INPUT_TYPE_CHAR:
				result = scan_char ( file, arg );
				break;
				
			case INPUT_TYPE_INT:
				result = scan_int ( file, arg );
				break;
				
			case INPUT_TYPE_FLOAT:
				result = scan_float ( file, arg );
				break;
				
			case INPUT_TYPE_OCTAL:
				result = scan_octal ( file, arg );
				break;
				
			case INPUT_TYPE_STRING:
				result = scan_string ( file, arg );
				break;
				
			case INPUT_TYPE_UINT:
				result = scan_unsigned_int ( file, arg );
				break;
				
			case INPUT_TYPE_HEX:
				result = scan_hex ( file, arg );
				break;
		}
	}
	return 0;
}
