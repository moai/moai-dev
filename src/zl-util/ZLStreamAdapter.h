// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLSTREAMADAPTER_H
#define ZLSTREAMADAPTER_H

#include <zl-util/ZLAccessors.h>
#include <zl-util/ZLStream.h>

//================================================================//
// ZLStreamProxy
//================================================================//
class ZLStreamProxy :
	public virtual ZLStream {
protected:

	ZLStream*		mProxiedStream;

public:

	GET_SET ( ZLStream*, ProxiedStream, mProxiedStream )

	//----------------------------------------------------------------//
	void			Compact					();
	void			Flush					();
	u32				GetCaps					();
	size_t			GetCursor				();
	size_t			GetLength				();
	bool			IsAtEnd					();
	ZLSizeResult	ReadBytes				( void* buffer, size_t size );
	ZLResultCode	SetCursor				( size_t offset );
	ZLSizeResult	SetLength				( size_t length );
	ZLSizeResult	WriteBytes				( const void* buffer, size_t size );
					ZLStreamProxy			();
					~ZLStreamProxy			();
};

//================================================================//
// ZLStreamAdapter
//================================================================//
class ZLStreamAdapter :
	public ZLStreamProxy {
protected:

	size_t			mBase;				// base of inner stream (when opened)
	
	size_t			mCursor;
	size_t			mLength;
	
	bool			mIsOpen;

	//----------------------------------------------------------------//
	virtual void			OnClose					();
	virtual ZLResultCode	OnOpen					();

public:

	//----------------------------------------------------------------//
	void					Close					();
	size_t					GetCursor				();
	size_t					GetLength				();
	bool					IsAtEnd					();
	ZLResultCode			Open					( ZLStream* stream = 0 );
							ZLStreamAdapter			();
							~ZLStreamAdapter		();
};

#endif
