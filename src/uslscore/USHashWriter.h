// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHASHWRITER_H
#define USHASHWRITER_H

#include <uslscore/USStreamWriter.h>

//================================================================//
// USHashWriter
//================================================================//
class USHashWriter :
	public USStreamWriter {
protected:	

	bool				mIsOpen;
	USStream*			mOutputStream;			// compressed output stream

	void*				mHMACKey;

	//----------------------------------------------------------------//
	virtual void		FinalizeHash			() = 0;
	virtual void		InitHash				() = 0;

public:

	//----------------------------------------------------------------//
	void				Close					();
	virtual size_t		GetBlockSize			() = 0;
	u32					GetCaps					();
	virtual u32			GetChecksum				();
	size_t				GetCursor				();
	virtual void*		GetHash					() = 0;
	virtual size_t		GetHashSize				() = 0;
	size_t				GetLength				();
	virtual void		HashBytes				( const void* buffer, size_t size ) = 0;
	bool				Open					( USStream* stream );
	void				SetHMACKey				( void* key, size_t keySize );
						~USHashWriter			();
						USHashWriter			();
	size_t				WriteBytes				( const void* buffer, size_t size );
};

#endif
