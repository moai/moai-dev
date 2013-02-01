// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USDATA_H
#define USDATA_H

#include <uslscore/USLeanArray.h>
#include <uslscore/USMutex.h>

class USCipher;
class USDataIOTask;
class USStreamReader;
class USStreamWriter;

//================================================================//
// USData
//================================================================//
class USData {
private:

	USMutex				mMutex;
	USLeanArray < u8 >	mBytes;

	//----------------------------------------------------------------//
	bool			Decode				( USStreamReader& reader );
	bool			Encode				( USStreamWriter& writer );

public:

	//----------------------------------------------------------------//
	bool			Base64Decode		();
	bool			Base64Encode		();
	void			Clear				();
	bool			Deflate				( int level, int windowBits );
	bool			HexDecode			();
	bool			HexEncode			();
	bool			Inflate				( int windowBits );
	bool			Load				( cc8* filename );
	void			Load				( void* bytes, size_t size );
	void			Lock				( void** bytes, size_t* size );
	bool			Save				( cc8* filename );
	void			Unlock				();
					USData				();
	virtual			~USData				();
};

#endif
