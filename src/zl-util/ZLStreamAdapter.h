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
	void			Flush					();
	u32				GetCaps					();
	size_t			GetCursor				();
	size_t			GetLength				();
	bool			IsAtEnd					();
	size_t			ReadBytes				( void* buffer, size_t size );
	int				SetCursor				( long offset );
	size_t			WriteBytes				( const void* buffer, size_t size );
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
	virtual void	OnClose					();
	virtual bool	OnOpen					();

public:

	//----------------------------------------------------------------//
	void			Close					();
	size_t			GetCursor				();
	size_t			GetLength				();
	bool			IsAtEnd					();
	bool			Open					( ZLStream* stream );
					ZLStreamAdapter			();
					~ZLStreamAdapter		();
};

#endif
