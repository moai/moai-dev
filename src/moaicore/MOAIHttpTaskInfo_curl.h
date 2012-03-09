// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHTTPTASKIMPL_H
#define MOAIHTTPTASKIMPL_H

#define  CURL_STATICLIB
#define  CURL_DISABLE_LDAP

extern "C" {
	#include <curl/curl.h>
}

//================================================================//
// MOAIHttpTaskInfo
//================================================================//
class MOAIHttpTaskInfo {
public:
	typedef STLMap<STLString, STLString> HeaderMap;
	typedef STLArray<STLString> HeaderList;
	
private:
	
	STLString			mUrl;
	CURL*				mEasyHandle;

	USMemStream			mMemStream;
	USByteStream		mByteStream;
	USLeanArray < u8 >	mData;
	
	USStream*			mStream;

	u32					mResponseCode;
	HeaderMap			mResponseHeaders;
	
	struct curl_slist*  mRequestHeaders;
	
	void*				mBody;

	friend class MOAIHttpTask;
	friend class MOAIUrlMgr;

	//----------------------------------------------------------------//
	static void		_printError				( CURLcode error );
	static u32		_writeData				( char* data, u32 n, u32 l, void* s );
	static u32		_writeHeader			( char* data, u32 n, u32 l, void* s );
	
	void			InitRequestHeaders		(HeaderList* extraHeaders);

	//----------------------------------------------------------------//
	void			Clear					();
	void			Finish					();
	void			InitForGet				( cc8* url, cc8* useragent, bool verbose, HeaderList *extraHeaders = 0 );
	void			InitForPost				( cc8* url, cc8* useragent, const void* buffer, u32 size, bool verbose, HeaderList *extraHeaders = 0);
					MOAIHttpTaskInfo		();
					~MOAIHttpTaskInfo		();
	void			PerformSync				();
};

#endif
