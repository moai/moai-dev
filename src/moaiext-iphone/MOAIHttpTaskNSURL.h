// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef libmoai_MOAIHttpTaskNsUrl_h
#define libmoai_MOAIHttpTaskNsUrl_h

//#ifdef USE_NSURL
#import <moaicore/moaicore.h>
#include <moaicore/MOAIHttpTaskBase.h>
#import <Foundation/Foundation.h> 

#define  CURL_STATICLIB
#define  CURL_DISABLE_LDAP

extern "C" {
#include <curl/curl.h>
}

@class MOAIHttpTaskNSURLDelegate;

//================================================================//
// MOAIHttpTaskNSURL
//================================================================//
/**	@name	MOAIHttpTaskNSURL
 @text	Implementation of MOAIHttpTask based on libcurl.
 */
class MOAIHttpTaskNSURL :
public MOAIHttpTaskBase {
private:
	
	friend class MOAIUrlMgrNSURL;
	
	STLString			mUrl;
	u32					mOpt;
	u32					mDefaultTimeout;
	NSURLConnection*	mEasyHandle;
	float				mExpectedLength;
	float				mDataReceived;
	//curl_slist*		mHeaderList;
	
	MOAIHttpTaskNSURLDelegate* mUrlDelegate;
	
	// This buffer holds data being sent *to* the server
	USLeanArray < u8 >	mBody;
	
	// The streams below are used to hold data coming *back* from the request
	
	// The USMemStream is used when the size is not know (USMemStream grows dynamically)
	USMemStream			mMemStream;
	
	// The USByteStream and data buffer is used when the size of the data is known in advance
	USByteStream		mByteStream;
	
	// This points to the stream being used
	USStream*			mStream;
	
	NSMutableData *data_;				/*!< Holds data for any data that comes back from a URL request. */
	NSURLConnection *connection_;		/*!< Used to provide support to perform the loading of a URL request. Delegate methods are defined to handle when a response is receive with associated data. This is used for asynchronous requests only. */
	int connectAttempts_;				/*!< The connect attempts is used to determine whether the alternate URL will be used. */
	
	
	
	//----------------------------------------------------------------//
	static u32		_writeData				( char* data, u32 n, u32 l, void* s );
	static u32		_writeHeader			( char* data, u32 n, u32 l, void* s );
	
	//----------------------------------------------------------------//
	void			AffirmHandle			();
	void			Clear					();
	void			CurlFinish				();
	void			Prepare					();
	//static void		PrintError				( CURLcode error );
	
public:
	
	DECL_LUA_FACTORY ( MOAIHttpTaskNSURL )
	
	//----------------------------------------------------------------//
	MOAIHttpTaskNSURL		();
	~MOAIHttpTaskNSURL		();
	void			PerformAsync			();
	void			PerformSync				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Reset					();
	void			SetBody					( const void* buffer, u32 size );
	void			SetFailOnError			( bool enable );
	void			SetCookieSrc			( const char *file );
	void			SetCookieDst			( const char *file );
	void			SetUrl					( cc8* url );
	void			SetUserAgent			( cc8* useragent );
	void			SetVerb					( int verb );
	void			SetVerbose				( bool verbose );
	
	
	void			didReceiveResponse		( int responseCode);
	void			didReceiveData			( const void* data, int size );
};


//================================================================//
// MOAITapjoyIOSDelegate
//================================================================//
@interface MOAIHttpTaskNSURLDelegate : NSObject {
@private
}
@end


//#endif
#endif
