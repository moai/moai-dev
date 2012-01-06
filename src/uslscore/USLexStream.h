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
	
	u32 mLine;
	u32 mLinePad;
	
	//----------------------------------------------------------------//
	u8			ReadByte			();
	u8			UnreadByte			();

public:
	
	GET ( u32, Line, mLine )
	
	//----------------------------------------------------------------//
	u32			GetCursor			();
	u32			GetLength			();
	u32			ReadBytes			( void* buffer, u32 size );
	void		Seek				( long offset, int origin );
	void		SetStream			( USStream* stream );
				USLexStream			();
				~USLexStream		();
	u32			WriteBytes			( const void* buffer, u32 size );
};

#endif
