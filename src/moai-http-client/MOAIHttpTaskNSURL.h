// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHTTPTASKNSURL_H
#define MOAIHTTPTASKNSURL_H

#import <moai-core/headers.h>
#include <moai-http-client/MOAIHttpTaskBase.h>
#import <Foundation/Foundation.h> 

@class MOAIHttpTaskNSURLDelegate;

//================================================================//
// MOAIHttpTaskNSURL
//================================================================//
/**	@lua	MOAIHttpTaskNSURL
 @text	Implementation of MOAIHttpTask based on libcurl.
 */
class MOAIHttpTaskNSURL :
	public MOAIHttpTaskBase {
private:
	
	friend class MOAIUrlMgrNSURL;
	
	STLString				mUrl;
	u32						mOpt;
	u32						mDefaultTimeout;
	float					mExpectedLength;
	float					mDataReceived;
	
	// This buffer holds data being sent *to* the server
	ZLLeanArray < u8 >		mBody;
	
	// The streams below are used to hold data coming *back* from the request
	
	// The USMemStream is used when the size is not know (USMemStream grows dynamically)
	ZLMemStream				mMemStream;
	
	// The USByteStream and data buffer is used when the size of the data is known in advance
	ZLByteStream			mByteStream;
	
	// This points to the stream being used
	ZLStream*				mStream;
	
	NSMutableData*			data_;				/* Holds data for any data that comes back from a URL request. */
	NSURLConnection*		connection_;		/* Used to provide support to perform the loading of a URL request. Delegate methods are defined to handle when a response is receive with associated data. This is used for asynchronous requests only. */
	int						connectAttempts_;	/* The connect attempts is used to determine whether the alternate URL will be used. */
	NSMutableURLRequest*	mRequest;
	
	
	//----------------------------------------------------------------//
	static u32			_writeData				( char* data, u32 n, u32 l, void* s );
	
	//----------------------------------------------------------------//
	void				Clear					();
	NSURLRequest*		Prepare					();
	
public:
	
	DECL_LUA_FACTORY ( MOAIHttpTaskNSURL )
	
	//----------------------------------------------------------------//
	void				DidFinishLoading		();
	void				DidReceiveData			( const void* data, int size );
	void				DidReceiveResponse		( int responseCode, NSDictionary* headers, int expectedLength );
	void				FinishRequest			();
						MOAIHttpTaskNSURL		();
						~MOAIHttpTaskNSURL		();
	void				PerformAsync			();
	void				PerformSync				();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				Reset					();
	void				SetBody					( const void* buffer, u32 size );
	void				SetFailOnError			( bool enable );
	void				SetCookieSrc			( const char *file );
	void				SetCookieDst			( const char *file );
	void				SetUrl					( cc8* url );
	void				SetUserAgent			( cc8* useragent );
	void				SetVerb					( int verb );
	void				SetVerbose				( bool verbose );
};


//================================================================//
// MOAITapjoyIOSDelegate
//================================================================//
@interface MOAIHttpTaskNSURLDelegate : NSObject {
@private
	MOAIHttpTaskNSURL*	mTask;
}

	//----------------------------------------------------------------//
	-( id )	initWithTask	:( MOAIHttpTaskNSURL* )task;
@end

#endif
