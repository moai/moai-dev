// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USPRETTYPRINT_H
#define USPRETTYPRINT_H

#define HEX_FORMAT "0x%08X"

#define BOOL_TO_STRING( _bool ) ( _bool ? "true" : "false" )

#define PRETTY_PRINT( str, member )	PrettyPrint ( str, #member, member );

//----------------------------------------------------------------//
template < typename T >
void PrettyPrint ( STLString& str, const char *name, const T& value ) {

	STLString tmp;
	const char *sep = ( str.size () == 0 ) ? "" : ", ";

	tmp.write ( "%s%s = " HEX_FORMAT, sep, name, &value );

	str.append ( tmp );
}

//----------------------------------------------------------------//
template < typename T >
void PrettyPrint ( STLString& str, const char *name, USRef< T >& value ) {

	PrettyPrint < T > ( str, name, *value );
}

//----------------------------------------------------------------//
// Prints the addresses of the elements within an array in the format:
//	{ 0xXXXXXXXX, 0xYYYYYYYY, 0xZZZZZZZZ }
template < typename T >
STLString LeanArrayAddressesToString ( const USLeanArray < T >& _array ) {

	STLString result ( "{ " );
	STLString tmp;

	int last = _array.Size () - 1;
	for ( int i = 0; i <= last; ++i ) {

		const char *sep = i == last ? " " : ", ";
		tmp.write ( HEX_FORMAT "%s", &_array [ i ], sep );
		result.append ( tmp );
	}

	return result + "}";
}

//----------------------------------------------------------------//
template < typename T >
void PrettyPrint ( STLString& str, const char *name, const USLeanArray < T >& value ) {

	STLString tmp;
	const char *sep = str.size () == 0 ? "" : ", ";

	tmp.write ( "%s%s = %s", sep, name, LeanArrayAddressesToString ( value ).c_str () );

	str.append ( tmp );
}

void PrettyPrint ( STLString& str, const char *name, bool value );
void PrettyPrint ( STLString& str, const char *name, const char *value );
void PrettyPrint ( STLString& str, const char *name, double value );
void PrettyPrint ( STLString& str, const char *name, float value );
void PrettyPrint ( STLString& str, const char *name, int value );
void PrettyPrint ( STLString& str, const char *name, u32 value );

void PrettyPrint ( STLString& str, const char *name, const STLString& value );
void PrettyPrint ( STLString& str, const char *name, const USRect& value );
void PrettyPrint ( STLString& str, const char *name, const USVec2D& value );

#endif
