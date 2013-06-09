// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLSTREAM_H
#define ZLSTREAM_H

#include <zl-util/STLString.h>

//================================================================//
// ZLStream
//================================================================//
class ZLStream {
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
	virtual void		Flush					();
	virtual u32			GetCaps					() = 0;
	virtual size_t		GetCursor				() = 0;
	virtual size_t		GetLength				();
	virtual bool		IsAtEnd					();
	size_t				PeekBytes				( void* buffer, size_t size );
	size_t				Print					( cc8* format, ... );
	size_t				Print					( cc8* format, va_list args );
	virtual size_t		ReadBytes				( void* buffer, size_t size );
	STLString			ReadString				( size_t size );
	STLString			ReadToken				( cc8* delimiters = 0 );
	int					Seek					( long offset, int origin );
	virtual int			SetCursor				( long offset );
	virtual size_t		WriteBytes				( const void* buffer, size_t size );
	size_t				WriteStream				( ZLStream& source );
	size_t				WriteStream				( ZLStream& source, size_t size );
						ZLStream				();
	virtual				~ZLStream				();

	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE Read ( TYPE value ) {
		TYPE temp;
		size_t result = this->ReadBytes ( &temp, sizeof ( TYPE ));
		if ( result == sizeof ( TYPE )) {
			value = temp;
		}
		return value;
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	void Write ( TYPE value ) {
		this->WriteBytes ( &value, sizeof ( TYPE ));
	}
};

//----------------------------------------------------------------//
template <> bool		ZLStream::Read < bool >			( bool value );
template <> void		ZLStream::Write < bool >		( bool value );

#endif
