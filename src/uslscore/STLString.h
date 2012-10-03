// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef XSTRING_H
#define	XSTRING_H

#include <uslscore/STLArray.h>

#include <string>

using namespace std;

#define STLSTRING_APPEND_VA_ARGS(str,format,top) \
	va_list args; \
	va_start ( args, top ); \
	str.write_var ( format, args ); \
	va_end ( args );

#define STLSTRING_FROM_VA_ARGS(str,format,top) \
	STLString str; \
	STLSTRING_APPEND_VA_ARGS ( str, format, top )

//================================================================//
// STLString
//================================================================//
class STLString :
	public string {
public:

	//----------------------------------------------------------------//
	void		base_64_decode		( void* buffer, u32 len );
	void		base_64_encode		( const void* buffer, u32 len );
	STLString	clip				( u32 first, u32 last );
	STLString	clip_to_back		( u32 first );
	STLString	clip_to_front		( u32 last );
	void		hex_encode			( const void* buffer, u32 len );
	static u8	hex_to_byte			( u32 c );
	void		replace_char		( cc8 match, cc8 sub );
	void		tokenize			( STLArray < STLString >& tokens, const STLString& delimiters = " " );
	double		to_double			();
	float		to_float			();
	int			to_int				();
	void		to_lower			();
	void		to_upper			();
	void		write				( cc8* format, ... );
	void		write_var			( cc8* format, va_list args );

	//----------------------------------------------------------------//
	inline operator const char* () const {
		return this->c_str ();
	}

	//----------------------------------------------------------------//
	inline char peek ( u32 id ) {
		if ( id >= this->length ()) {
			return 0x00;
		}
		return this->at ( id );
	}

	//----------------------------------------------------------------//
	inline const char* str () const {
		return this->c_str ();
	}
	
	//----------------------------------------------------------------//
	inline STLString () :
		string () {
	}

	//----------------------------------------------------------------//
	inline STLString ( const string& s2 ) :
		string ( s2 ) {
	}

	//----------------------------------------------------------------//
	inline STLString ( const string& s2, size_type pos2, size_type len2 ) :
		string ( s2, pos2, len2 ) {
	}

	//----------------------------------------------------------------//
	inline STLString ( const char* nts ) :
		string ( nts ) {
	}

	//----------------------------------------------------------------//
	inline STLString ( const char* buf, size_type bufsize ) :
		string ( buf, bufsize ) {
	}

	//----------------------------------------------------------------//
	inline STLString ( size_type repetitions, char c ) :
		string ( repetitions, c ) {
	}

	//----------------------------------------------------------------//
	STLString ( int value, int radix = 10 );

	//----------------------------------------------------------------//
	STLString ( double value );
};

#endif
