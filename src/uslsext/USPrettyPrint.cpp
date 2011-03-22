// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USPrettyPrint.h>

//----------------------------------------------------------------//
void PrettyPrint ( STLString& str, const char *name, bool value ) {

	STLString tmp;
	const char *sep = str.size () == 0 ? "" : ", ";

	tmp.write ( "%s%s = %s", sep, name, BOOL_TO_STRING( value ) );

	str.append ( tmp );
}

//----------------------------------------------------------------//
void PrettyPrint ( STLString& str, const char *name, const char *value ) {

	STLString tmp;
	const char *sep = str.size () == 0 ? "" : ", ";

	tmp.write ( "%s%s = %s", sep, name, value );

	str.append ( tmp );
}

//----------------------------------------------------------------//
void PrettyPrint ( STLString& str, const char *name, double value ) {

	STLString tmp;
	const char *sep = str.size () == 0 ? "" : ", ";

	tmp.write ( "%s%s = %f", sep, name, value );

	str.append ( tmp );
}

//----------------------------------------------------------------//
void PrettyPrint ( STLString& str, const char *name, float value ) {

	STLString tmp;
	const char *sep = str.size () == 0 ? "" : ", ";

	tmp.write ( "%s%s = %f", sep, name, value );

	str.append ( tmp );
}

//----------------------------------------------------------------//
void PrettyPrint ( STLString& str, const char *name, int value ) {

	STLString tmp;
	const char *sep = str.size () == 0 ? "" : ", ";

	tmp.write ( "%s%s = %d", sep, name, value );

	str.append ( tmp );
}

//----------------------------------------------------------------//
void PrettyPrint ( STLString& str, const char *name, u32 value ) {

	PrettyPrint ( str, name, static_cast < int >( value ) );
}

//----------------------------------------------------------------//
void PrettyPrint ( STLString& str, const char *name, const USRect& value ) {

	STLString tmp;
	const char *sep = str.size () == 0 ? "" : ", ";

	tmp.write ( "%s%s = Rect { left: %f, top: %f, width: %f, height: %f }", sep, name, value.mXMin, value.mYMin, value.Width (), value.Height () );

	str.append ( tmp );
}

//----------------------------------------------------------------//
void PrettyPrint ( STLString& str, const char *name, const STLString& value ) {

	PrettyPrint ( str, name, value.c_str () );
}

//----------------------------------------------------------------//
void PrettyPrint ( STLString& str, const char *name, const USVec2D& value ) {

	STLString tmp;
	const char *sep = str.size () == 0 ? "" : ", ";

	tmp.write ( "%s%s = Vec2D { %f, %f }", sep, name, value.mX, value.mY );

	str.append ( tmp );
}