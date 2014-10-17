// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHTTPTASKCURL_H
#define MOAIHTTPTASKCURL_H

#if MOAI_WITH_LIBCURL

#include <moai-http-client/MOAIHttpTaskBase.h>

#define  CURL_STATICLIB
#define  CURL_DISABLE_LDAP

extern "C" {
	#include <curl/curl.h>
}

//================================================================//
// MOAIHttpTaskCurl
//================================================================//
/**	@lua	MOAIHttpTaskCurl
	@text	Implementation of MOAIHttpTask based on libcurl.
*/
class MOAIHttpTaskCurl :
	public MOAIHttpTaskBase {
private:
	
	friend class MOAIUrlMgrCurl;

	STLString			mUrl;
	u32					mDefaultTimeout;
	//u32					m
	CURL*				mEasyHandle;
	curl_slist*			mHeaderList;

	// This buffer holds data being sent *to* the server
	ZLLeanArray < u8 >	mBody;
	
	// The streams below are used to hold data coming *back* from the request
	
	// The ZLMemStream is used when the size is not known (ZLMemStream grows dynamically)
	ZLMemStream			mMemStream;
	
	// The ZLByteStream and data buffer is used when the size of the data is known in advance
	ZLByteStream		mByteStream;
	
	// This points to the stream being used
	ZLStream*			mStream;

	//----------------------------------------------------------------//
	static u32		_writeData				( char* data, u32 n, u32 l, void* s );
	static u32		_writeHeader			( char* data, u32 n, u32 l, void* s );
	static u32		_progressFunction		( void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded );

	//----------------------------------------------------------------//
	void			AffirmHandle			();
	void			Clear					();
	void			CurlFinish				();
	void			Prepare					();
	static void		PrintError				( CURLcode error );

public:

	DECL_LUA_FACTORY ( MOAIHttpTaskCurl )

	//----------------------------------------------------------------//
					MOAIHttpTaskCurl		();
					~MOAIHttpTaskCurl		();
	void			PerformAsync			();
	void			PerformSync				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Reset					();
	void			SetBody					( const void* buffer, u32 size );
	void			SetCookieSrc			( const char *file );
	void			SetCookieDst			( const char *file );
	void			SetFailOnError			( bool enable );
	void			SetUrl					( cc8* url );
	void			SetUserAgent			( cc8* useragent );
	void			SetVerb					( int verb );
	void			SetVerbose				( bool verbose );
};

#endif
#endif
