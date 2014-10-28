// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHASHWRITER_H
#define ZLHASHWRITER_H

#include <zl-util/ZLStreamAdapter.h>

//================================================================//
// ZLHashWriter
//================================================================//
class ZLHashWriter :
	public ZLStreamAdapter {
protected:	

	void*				mHMACKey;

	//----------------------------------------------------------------//
	virtual void		FinalizeHash			() = 0;
	virtual void		InitHash				() = 0;
	void				OnClose					();
	bool				OnOpen					();
	size_t				WriteBytes				( const void* buffer, size_t size );

public:

	//----------------------------------------------------------------//
	virtual size_t		GetBlockSize			() = 0;
	u32					GetCaps					();
	virtual u32			GetChecksum				();
	virtual void*		GetHash					() = 0;
	virtual size_t		GetHashSize				() = 0;
	virtual void		HashBytes				( const void* buffer, size_t size ) = 0;
	void				SetHMACKey				( void* key, size_t keySize );
						~ZLHashWriter			();
						ZLHashWriter			();
};

#endif
