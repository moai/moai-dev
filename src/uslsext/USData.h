// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USDATA_H
#define USDATA_H

#include <uslsext/USMutex.h>

class USDataIOTask;

//================================================================//
// USData
//================================================================//
class USData {
private:

	USMutex				mMutex;
	USLeanArray < u8 >	mBytes;

	//----------------------------------------------------------------//

public:

	//----------------------------------------------------------------//
	void			Clear				();
	bool			Load				( cc8* filename );
	void			Load				( void* bytes, u32 size );
	void			Lock				( void** bytes, u32* size );
	bool			Save				( cc8* filename, bool affirm_path = true );
	void			Unlock				();
					USData				();
	virtual			~USData				();
};

#endif
