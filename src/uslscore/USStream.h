// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USSTREAM_H
#define USSTREAM_H

#include <uslscore/STLString.h>

//================================================================//
// USStream
//================================================================//
class USStream {
protected:

	static const u32 LOCAL_BUFFER = 1024;
	static const size_t UNKNOWN_SIZE = ( size_t )-1;

public:

	enum {
		CAN_READ	= 0x01,
		CAN_WRITE	= 0x02,
		CAN_SEEK	= 0x04,
	};

	//----------------------------------------------------------------//
	virtual void		Flush			();
	virtual u32			GetCaps			() = 0;
	virtual size_t		GetCursor		() = 0;
	virtual size_t		GetLength		();
	virtual bool		IsAtEnd			();
	size_t				PeekBytes		( void* buffer, size_t size );
	size_t				Pipe			( USStream& source );
	size_t				Pipe			( USStream& source, size_t size );
	size_t				Print			( cc8* format, ... );
	size_t				Print			( cc8* format, va_list args );
	virtual size_t		ReadBytes		( void* buffer, size_t size );
	STLString			ReadStr			( u32 size );
	int					Seek			( long offset, int origin );
	virtual int			SetCursor		( long offset );
						USStream		();
	virtual				~USStream		();
	virtual size_t		WriteBytes		( const void* buffer, size_t size );

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
