// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USMD5WRITER_H
#define USMD5WRITER_H

#include <openssl/md5.h>
#include <uslscore/USStreamWriter.h>

//================================================================//
// USMD5Writer
//================================================================//
class USMD5Writer :
	public USStreamWriter {
private:
		
	USStream*			mOutputStream;			// compressed output stream
	size_t				mCursor;				// cursor in the input stream
		
	MD5_CTX*			mMD5Context;
	u8*					mFinalMD5;
		
public:

	//----------------------------------------------------------------//
	void				Close					();
	u32					GetCaps					();
	u8*					GetHash					();
	size_t				GetCursor				();
	size_t				GetLength				();
	bool				Open					( USStream& stream );
						~USMD5Writer			();
						USMD5Writer				();
	size_t				WriteBytes				( const void* buffer, size_t size );
};

#endif
