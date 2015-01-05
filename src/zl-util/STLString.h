// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef STLSTRING_H
#define	STLSTRING_H

#include <zl-util/STLArray.h>

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

class ZLStreamAdapter;
class ZLStreamAdapter;

//================================================================//
// STLString
//================================================================//
class STLString :
	public string {
	
	//----------------------------------------------------------------//
	void				zl_decode			( ZLStreamAdapter& reader, void* buffer, size_t len );
	void				zl_encode			( ZLStreamAdapter& writer, const void* buffer, size_t len );
	
public:

	//----------------------------------------------------------------//
	STLString&			assign				( cc8* str );
	void				base_64_decode		( void* buffer, size_t len );
	size_t				base_64_decode_len	(); // calc the *approx* len of a plain str; safe to use for buffer allocation
	void				base_64_encode		( const void* buffer, size_t len );
	size_t				base_64_encode_len	(); // calc the *approx* len of an encoded str; safe to use for buffer allocation
	
	static STLString	build				( cc8* format, ... );
	static STLString	build_var			( cc8* format, va_list args );
	
	STLString			clip				( size_t first, size_t last );
	STLString			clip_to_back		( size_t first );
	STLString			clip_to_front		( size_t last );

	void				hex_decode			( void* buffer, size_t len );
	size_t				hex_decode_len		(); // calc the len of a plain str
	void				hex_encode			( const void* buffer, size_t len );
	size_t				hex_encode_len		(); // calc the len of an encoded str
	
	static u8			hex_to_byte			( u32 c );
	
	void				replace_char		( cc8 match, cc8 sub );
	size_t				tokenize			( STLArray < STLString >& tokens, const STLString& delimiters = " " ) const;
	static size_t		tokenize			( const STLString& str, STLArray < STLString >& tokens, const STLString& delimiters = " " );
	double				to_double			();
	float				to_float			();
	int					to_int				();
	void				to_lower			();
	void				to_upper			();
	void				write				( cc8* format, ... );
	void				write_var			( cc8* format, va_list args );

	void				zip_deflate			( const void* buffer, size_t len ); // deflate then base64 encode
	size_t				zip_inflate			( void* buffer, size_t len ); // base64 decode then inflate

	//----------------------------------------------------------------//
	inline STLString& operator= ( cc8* rhs ) {
		return this->assign ( rhs );
	}

	//----------------------------------------------------------------//
	inline STLString& operator= ( const string& rhs ) {
		this->string::assign ( rhs );
		return *this;
	}

	//----------------------------------------------------------------//
	inline STLString& operator= ( const STLString& rhs ) {
		this->string::assign ( rhs );
		return *this;
	}

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
	inline STLString ( const STLString& s2 ) :
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
