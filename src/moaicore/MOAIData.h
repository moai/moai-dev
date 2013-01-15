// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDATA_H
#define MOAIDATA_H

#include <moaicore/MOAIMutex.h>

class USStreamReader;
class USStreamWriter;

//================================================================//
// MOAIData
//================================================================//
class MOAIData {
private:

	MOAIMutex			mMutex;
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
					MOAIData			();
	virtual			~MOAIData			();
	bool			Save				( cc8* filename );
	void			Unlock				();
};

#endif
