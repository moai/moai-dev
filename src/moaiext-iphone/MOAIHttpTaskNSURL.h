//
//  MOAIHttpTaskNsUrl.h
//  libmoai
//
//  Created by Megan Peterson on 7/28/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef libmoai_MOAIHttpTaskNsUrl_h
#define libmoai_MOAIHttpTaskNsUrl_h

#ifdef USE_NSURL

#include <moaicore/MOAIHttpTaskBase.h>

#define  CURL_STATICLIB
#define  CURL_DISABLE_LDAP

extern "C" {
#include <curl/curl.h>
}

//================================================================//
// MOAIHttpTaskCurl
//================================================================//
/**	@name	MOAIHttpTaskCurl
 @text	Implementation of MOAIHttpTask based on libcurl.
 */
class MOAIHttpTaskNSURL :
public MOAIHttpTaskBase {
private:
	
	friend class MOAIUrlMgrCurl;
	
	STLString			mUrl;
	u32					mDefaultTimeout;
	CURL*				mEasyHandle;
	curl_slist*			mHeaderList;
	
	MOAIHttpTaskNSURLDelegate mUrlDelegate;
	
	// This buffer holds data being sent *to* the server
	USLeanArray < u8 >	mBody;
	
	// The streams below are used to hold data coming *back* from the request
	
	// The USMemStream is used when the size is not know (USMemStream grows dynamically)
	USMemStream			mMemStream;
	
	// The USByteStream and data buffer is used when the size of the data is known in advance
	USByteStream		mByteStream;
	
	// This points to the stream being used
	USStream*			mStream;
	
	//----------------------------------------------------------------//
	static u32		_writeData				( char* data, u32 n, u32 l, void* s );
	static u32		_writeHeader			( char* data, u32 n, u32 l, void* s );
	
	//----------------------------------------------------------------//
	void			AffirmHandle			();
	void			Clear					();
	void			CurlFinish				();
	void			Prepare					();
	static void		PrintError				( CURLcode error );
	
public:
	
	DECL_LUA_FACTORY ( MOAIHttpTaskCurl )
	
	//----------------------------------------------------------------//
	MOAIHttpTaskNSURL		();
	~MOAIHttpTaskNSURL		();
	void			PerformAsync			();
	void			PerformSync				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Reset					();
	void			SetBody					( const void* buffer, u32 size );
	void			SetCookieSrc			( const char *file );
	void			SetCookieDst			( const char *file );
	void			SetUrl					( cc8* url );
	void			SetUserAgent			( cc8* useragent );
	void			SetVerb					( u32 verb );
	void			SetVerbose				( bool verbose );
};


//================================================================//
// MOAITapjoyIOSDelegate
//================================================================//
@interface MOAIHttpTaskNSURLDelegate : NSObject {
@private
}
@end



#endif
#endif
