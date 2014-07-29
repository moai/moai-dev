// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHTTPTASKNACL_H
#define MOAIHTTPTASKNACL_H

#include <moai-http-client/MOAIHttpTaskBase.h>

#ifdef MOAI_OS_NACL

#include "geturl_handler.h"

//================================================================//
// MOAIHttpTaskNaCl
//================================================================//
/**	@lua	MOAIHttpTaskNaCl
	@text	Implementation of MOAIHttpTask based on NaCl.
*/
class MOAIHttpTaskNaCl :
	public MOAIHttpTaskBase {
private:
	STLString			mUrl;
	ZLLeanArray < u8 >	mBody;
	int mMethod;

	ZLMemStream			mMemStream;
	ZLByteStream		mByteStream;
		
	// This points to the stream being used
	ZLStream*			mStream;
	
	bool				mReady;

	bool				mLock;

	const void *		mTempBufferToCopy;
	int					mTempBufferToCopySize;

	friend class MOAIUrlMgr;

	static void HttpLoaded ( GetURLHandler *handler, const char *buffer, int32_t size );
	static void HttpGetMainThread ( void* userData, int32_t result );

	void Prepare ( GetURLHandler *handler );


public:

	DECL_LUA_FACTORY ( MOAIHttpTaskNaCl )

	//----------------------------------------------------------------//
	void			Clear					();
	bool			IsReady					();
					MOAIHttpTaskNaCl		();
					~MOAIHttpTaskNaCl		();
	void			NaClFinish				();
	void			PerformAsync			();
	void			PerformSync				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Reset					();
	void			SetBody					( const void* buffer, u32 size );
	void			SetCookieDst			( const char *file );
	void			SetCookieSrc			( const char *file );
	void			SetFailOnError			( bool enable );
	void			SetUrl					( cc8* url );
	void			SetUserAgent			( cc8* useragent );
	void			SetVerb					( int verb );
	void			SetVerbose				( bool verbose );
};

#endif
#endif
