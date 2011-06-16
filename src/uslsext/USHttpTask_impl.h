// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USHTTPTASKIMPL_H
#define USHTTPTASKIMPL_H

#define  CURL_STATICLIB
#define  CURL_DISABLE_LDAP

extern "C" {
	#include <curl/curl.h>
}

//================================================================//
// USHttpTaskInfo
//================================================================//
class USHttpTaskInfo {
private:
	
	STLString			mUrl;
	CURL*				mEasyHandle;

	USMemStream			mMemStream;
	USByteStream		mByteStream;
	USLeanArray < u8 >	mData;
	
	USStream*			mStream;

	u32					mResponseCode;

	friend class USHttpTask;
	friend class USUrlMgr;

	//----------------------------------------------------------------//
	static void		_printError				( CURLcode error );
	static u32		_writeData				( char* data, u32 n, u32 l, void* s );
	static u32		_writeHeader			( char* data, u32 n, u32 l, void* s );

	//----------------------------------------------------------------//
	void			Clear					();
	void			Finish					();
	void			InitForGet				( cc8* url, cc8* useragent, bool verbose );
	void			InitForPost				( cc8* url, cc8* useragent, const void* buffer, u32 size, bool verbose );
					USHttpTaskInfo			();
					~USHttpTaskInfo			();
};

#endif
