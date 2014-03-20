// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLLEXSTREAM_H
#define ZLLEXSTREAM_H

#include <zl-util/ZLAccessors.h>
#include <zl-util/ZLStream.h>

//================================================================//
// ZLLexStream
//================================================================//
class ZLLexStream :
	public ZLStream {
private:
	
	ZLStream* mStream;
	
	static const u32 DEFAULT_LINE_NO	= 1;
	static const u32 DEFAULT_LINE_PAD	= 1;
	
	size_t mLine;
	
	//----------------------------------------------------------------//
	u8			ReadByte			();
	int			SetCursor			( long offset );
	u8			UnreadByte			();

public:
	
	GET ( size_t, Line, mLine )
	
	//----------------------------------------------------------------//
	u32			GetCaps				();
	size_t		GetCursor			();
	size_t		GetLength			();
	size_t		ReadBytes			( void* buffer, size_t size );
	void		SetStream			( ZLStream* stream );
				ZLLexStream			();
				~ZLLexStream		();
};

#endif
