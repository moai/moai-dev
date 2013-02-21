// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USLEXSTREAM_H
#define USLEXSTREAM_H

#include <uslscore/USAccessors.h>
#include <uslscore/USStream.h>

//================================================================//
// USLexStream
//================================================================//
class USLexStream :
	public USStream {
private:
	
	USStream* mStream;
	
	static const u32 DEFAULT_LINE_NO	= 1;
	static const u32 DEFAULT_LINE_PAD	= 1;
	
	size_t mLine;
	
	//----------------------------------------------------------------//
	u8			ReadByte			();
	int			SetCursor			( long offset );
	u8			UnreadByte			();

public:
	
	GET ( u32, Line, mLine )
	
	//----------------------------------------------------------------//
	u32			GetCaps				();
	size_t		GetCursor			();
	size_t		GetLength			();
	size_t		ReadBytes			( void* buffer, size_t size );
	void		SetStream			( USStream* stream );
				USLexStream			();
				~USLexStream		();
};

#endif
