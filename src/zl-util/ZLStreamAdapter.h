// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLSTREAMADAPTER_H
#define ZLSTREAMADAPTER_H

#include <zl-util/ZLStream.h>

//================================================================//
// ZLStreamAdapter
//================================================================//
class ZLStreamAdapter :
	public ZLStream {
protected:

	ZLStream*		mStream;
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
