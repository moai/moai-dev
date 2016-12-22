// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLSTREAM_H
#define ZLSTREAM_H

#include <zl-util/STLString.h>
#include <zl-util/ZLResult.h>

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
		CAN_TRUNC	= 0x08,
	};

	//----------------------------------------------------------------//
	bool						CheckCaps				( u32 flags );
	ZLSizeResult				Collapse				( size_t clipBase, size_t clipSize, size_t chunkSize, size_t size, bool invert );
	ZLSizeResult				Collapse				( ZLStream& source, size_t clipBase, size_t clipSize, size_t chunkSize, size_t size, bool invert );
	virtual void				Compact					();
	virtual void				Flush					();
	virtual u32					GetCaps					() = 0;
	virtual size_t				GetCursor				() = 0;
	virtual size_t				GetLength				();
	virtual bool				IsAtEnd					();
	ZLSizeResult				PeekBytes				( void* buffer, size_t size );
	ZLSizeResult				Print					( cc8* format, ... );
	ZLSizeResult				Print					( cc8* format, va_list args );
	virtual ZLSizeResult		ReadBytes				( void* buffer, size_t size );
	ZLStringResult				ReadString				( size_t size );
	ZLStringResult				ReadToken				( cc8* delimiters = 0 );
	float						Sample					( u32 streamType, size_t sampleSize );
	int							Seek					( long offset, int origin = SEEK_SET );
	virtual ZLResultCode		SetCursor				( size_t offset );
	virtual ZLSizeResult		SetLength				( size_t length );
	virtual ZLSizeResult		WriteBytes				( const void* buffer, size_t size );
	ZLSizeResult				WriteStream				( ZLStream& source );
	ZLSizeResult				WriteStream				( ZLStream& source, size_t size );
								ZLStream				();
	virtual						~ZLStream				();

	//----------------------------------------------------------------//
	template < typename TYPE >
	ZLResult < TYPE > Read () {
		TYPE value;
		ZLSizeResult result = this->ReadBytes ( &value, sizeof ( TYPE ));
		ZL_RETURN_RESULT ( TYPE, value, result.mCode );
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	ZLResult < TYPE > Read ( TYPE fallback ) {
		TYPE value;
		if ( this->ReadBytes ( &value, sizeof ( TYPE )).mValue == sizeof ( TYPE )) {
			ZL_RETURN_RESULT ( TYPE, value, ZL_OK );
		}
		ZL_RETURN_RESULT ( TYPE, fallback, ZL_ERROR );
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	ZLResultCode Read ( TYPE& value, const TYPE& fallback ) {
		TYPE temp;
		if ( this->ReadBytes ( &temp, sizeof ( TYPE )).mValue == sizeof ( TYPE )) {
			value = temp;
			return ZL_OK;
		}
		value = fallback;
		return ZL_ERROR;
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	ZLSizeResult Write ( TYPE value ) {
		return this->WriteBytes ( &value, sizeof ( TYPE ));
	}
};

//----------------------------------------------------------------//
template <> ZLResult < bool >	ZLStream::Read < bool >			();
template <> ZLResult < bool >	ZLStream::Read < bool >			( bool value );
template <> ZLSizeResult		ZLStream::Write < bool >		( bool value );

#endif
