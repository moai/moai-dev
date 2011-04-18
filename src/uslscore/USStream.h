// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef STREAM_H
#define STREAM_H

#include <uslscore/STLString.h>

//================================================================//
// USStream
//================================================================//
class USStream {
protected:

	static const u32 LOCAL_BUFFER = 1024;

public:

	//----------------------------------------------------------------//
	virtual bool		Done			();
	virtual void		Flush			();
	virtual u32			GetCursor		() = 0;
	virtual u32			GetLength		() = 0;
	u32					Pipe			( USStream& source );
	u32					Pipe			( USStream& source, u32 size );
	u32					Print			( cc8* format, ... );
	u32					Print			( cc8* format, va_list args );
	virtual u32			ReadBytes		( void* buffer, u32 size ) = 0;
	STLString			ReadStr			( u32 size );
	virtual void		Seek			( long offset, int origin ) = 0;
						USStream		();
	virtual				~USStream		();
	virtual u32			WriteBytes		( const void* buffer, u32 size ) = 0;

	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE Read () {
		TYPE value;
		this->ReadBytes ( &value, sizeof ( TYPE ));
		return value;
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	void Write ( TYPE value ) {
		this->WriteBytes ( &value, sizeof ( TYPE ));
	}
};

//----------------------------------------------------------------//
template <> bool		USStream::Read < bool >			();
template <> string		USStream::Read < string >		();
template <> STLString	USStream::Read < STLString >	();
template <> void		USStream::Write < bool >		( bool value );
template <> void		USStream::Write < string >		( string value );
template <> void		USStream::Write < STLString >	( STLString value );

#endif
