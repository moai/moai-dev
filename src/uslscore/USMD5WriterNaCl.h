// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USMD5WRITER_H
#define USMD5WRITER_H

#include <uslscore/USStreamWriter.h>

//================================================================//
// USMD5WriterNaCl
//================================================================//
class USMD5Writer :
	public USStreamWriter {
private:
		
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
