// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <iostream>

#include "pch.h"

#include <algorithm>
#include <moai-http-client/MOAIHttpTaskNSURL.h>

#define MAX_HEADER_LENGTH 1024

//================================================================//
// cpp helper
//================================================================//

//----------------------------------------------------------------//
void RegisterMOAIHttpTaskNSURL () {
	REGISTER_LUA_CLASS ( MOAIHttpTaskNSURL );
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
u32 MOAIHttpTaskNSURL::_writeData ( char* data, u32 n, u32 l, void* s ) {
	
	MOAIHttpTaskNSURL* self = ( MOAIHttpTaskNSURL* )s;
	u32 size = n * l;
	
	self->mStream->WriteBytes ( data, size );
	return size;
	 
	return 0;
}

//================================================================//
// MOAIHttpTaskNSURL
//================================================================//

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::Clear () {
	
	this->mUrl.clear ();
	this->mBody.Clear ();
	this->mMemStream.Clear ();
	this->mData.Clear ();
	this->mResponseHeaders.clear();
	
	this->mResponseCode = 0;
	this->mStream = 0;
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::DidFinishLoading () {

	this->FinishRequest ();
	this->LatchRelease ();
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::DidReceiveData ( const void* data, int size ) {

	this->mDataReceived += size;
	this->mProgress = this->mDataReceived / this->mExpectedLength;
	MOAIHttpTaskNSURL::_writeData (( char* )data, size, 1, this );
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::DidReceiveResponse ( int responseCode, NSDictionary* headers, int expectedLength ) {

	this->mResponseCode = responseCode;
	this->mExpectedLength = expectedLength;
	
	for ( id key in headers ) {
		this->mResponseHeaders [[ key UTF8String ]] = [[ headers objectForKey:key ] UTF8String ];
	}
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::FinishRequest () {
	
	if ( this->mStream == &this->mMemStream ) {
		
		u32 size = this->mMemStream.GetLength ();
		
		if ( size ) {
			this->mData.Init ( size );
			this->mStream->Seek ( 0, SEEK_SET );
			this->mStream->ReadBytes ( this->mData, size );
		}
		this->mMemStream.Clear ();
	}
	this->Finish ();
}

//----------------------------------------------------------------//
MOAIHttpTaskNSURL::MOAIHttpTaskNSURL () :
	mOpt ( 0 ),
	mDefaultTimeout ( 10 ),
	mDataReceived ( 0 ),
	mStream ( 0 ),
	mRequest ( 0 ) {
	
	RTTI_SINGLE ( MOAIHttpTaskBase )
}

//----------------------------------------------------------------//
MOAIHttpTaskNSURL::~MOAIHttpTaskNSURL () {
	
	this->Clear ();
}

//----------------------------------------------------------------//
NSURLRequest* MOAIHttpTaskNSURL::Prepare () {
	
	// until we get a header indicating otherwise, assume we won't
	// know the final length of the stream, so default to use the
	// ZLMemStream which will grow dynamically
	if ( this->mUserStream ) {
		this->mStream = this->mUserStream;
	}
	else {
		this->mStream = &this->mMemStream;
	}
	
	NSString* requestString = [ NSString stringWithCString:mUrl encoding:NSUTF8StringEncoding ];
	NSURL* myURL = [ NSURL URLWithString:requestString ];
	NSMutableURLRequest* request = [ NSMutableURLRequest requestWithURL: myURL cachePolicy: NSURLRequestReloadIgnoringLocalCacheData timeoutInterval: mTimeout ];
	
	switch ( this->mOpt ) {
				
		case HTTP_GET:
			[ request setHTTPMethod:@"GET" ];
			break;
				
		case HTTP_HEAD:
			[ request setHTTPMethod:@"HEAD" ];
			break;
				
		case HTTP_POST:
			[ request setHTTPMethod:@"POST" ];
			break;
				
		case HTTP_PUT:
			[ request setHTTPMethod:@"PUT" ];
			break;
				
		case HTTP_DELETE:
			[ request setHTTPMethod:@"DELETE" ];
			break;
	}
	
	if ( this->mHeaderMap.size() > 0 ) {
		for ( auto& pair: this->mHeaderMap ) {
			NSString* value = [ NSString stringWithUTF8String:pair.second ];
			NSString* key = [ NSString stringWithUTF8String:pair.first ];
			[ request addValue:value forHTTPHeaderField:key ];
		}
	}
	
	if ( mBody.Size () > 0 ) {
		[ request setHTTPBody:[ NSData dataWithBytes:mBody.Data () length:mBody.Size ()]];
		mBody.Clear ();
	}
	
	return request;
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::PerformAsync () {
	
	this->LatchRetain ();

	NSURLRequest* request = this->Prepare ();
	MOAIHttpTaskNSURLDelegate* delegate = [[[ MOAIHttpTaskNSURLDelegate alloc ] initWithTask:this ] autorelease ];
	
	[ NSURLConnection connectionWithRequest:request delegate:delegate ];
	//dispatch_async ( dispatch_get_main_queue(), ^{ connection_ = [ this->mEasyHandle initWithRequest: this->mRequest delegate: mUrlDelegate ]; });
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::PerformSync () {
	
	NSHTTPURLResponse* myResponse = nil;
	NSError* myError = nil;
	
	NSURLRequest* request = this->Prepare ();

	NSData* data = [ NSURLConnection sendSynchronousRequest:request returningResponse:&myResponse error:&myError ];
    this->mResponseCode = [ myResponse statusCode ];
	
	if ( myError == NULL ) {
		if (data) {
			_writeData (( char* )data.bytes, data.length, 1, this );
		}
	}
	else {
		NSLog ( @"Error: %@ %@", myError, [ myError userInfo ]);
	}
	
	this->FinishRequest ();
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIHttpTaskBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIHttpTaskBase::RegisterLuaFuncs ( state );
	
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::Reset () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetBody ( const void* buffer, u32 size ) {
	
	this->mBody.Init ( size );
	memcpy ( this->mBody, buffer, size );
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetFailOnError ( bool enable ) {
	
	UNUSED(enable);
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetCookieDst	( const char *file ) {
	
	UNUSED(file);
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetCookieSrc	( const char *file ) {
	
	UNUSED(file);
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetUrl ( cc8* url ) {
	
	this->mUrl = url;
	
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetUserAgent ( cc8* useragent ) {
	
	UNUSED(useragent);
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetVerb ( int verb ) {
	
	this->mOpt = verb;
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetVerbose ( bool verbose ) {
	
	UNUSED(verbose);
}



//================================================================//
// MOAIHttpTaskNSURLDelegate
//================================================================//
@implementation MOAIHttpTaskNSURLDelegate

	//================================================================//
	#pragma mark -
	#pragma mark Protocol MOAIHttpTaskNSURLDelegate
	//================================================================//

	#pragma mark delegate methods for asynchronous requests

	//----------------------------------------------------------------//
	-( BOOL )connection:( NSURLConnection* )connection canAuthenticateAgainstProtectionSpace:(NSURLProtectionSpace *)protectionSpace {
		UNUSED(connection);
		
		return [protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust];
	}

	//----------------------------------------------------------------//
	-( void )connection:( NSURLConnection* )connection didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge {
		UNUSED(connection);
		
		//if ([challenge.protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust])
		//	if (... user allows connection despite bad certificate ...)
				[ challenge.sender useCredential:[NSURLCredential credentialForTrust:challenge.protectionSpace.serverTrust] forAuthenticationChallenge:challenge ];
		
		[ challenge.sender continueWithoutCredentialForAuthenticationChallenge:challenge ];
	}

	//----------------------------------------------------------------//
	- (void)connection:(NSURLConnection*) myConnection didReceiveResponse:(NSURLResponse*) myResponse {
		UNUSED ( myConnection );
	
		NSDictionary* headers = [( NSHTTPURLResponse* )myResponse allHeaderFields ];
		self->mTask->DidReceiveResponse ([( NSHTTPURLResponse* )myResponse statusCode ], headers, ( int )[( NSHTTPURLResponse* )myResponse expectedContentLength ]);
	}

	//----------------------------------------------------------------//
	- (void)connection:(NSURLConnection*) myConnection didReceiveData:(NSData*) myData {
		UNUSED ( myConnection );
	
		self->mTask->DidReceiveData ( myData.bytes, myData.length );
	}

	//----------------------------------------------------------------//
	- (void)connection:(NSURLConnection*) myConnection didFailWithError:(NSError*) myError {
		UNUSED ( myConnection );
	
		NSLog ( @"Error: %@ %@", myError, [ myError userInfo ]);
		self->mTask->DidFinishLoading ();
	}

	//----------------------------------------------------------------//
	-( id )	initWithTask :( MOAIHttpTaskNSURL* )task {
	
		self = [ super init ];
		if ( self != nil ) {
			self->mTask = task;
		}
		return self;
	}

	//----------------------------------------------------------------//
	- (void)connectionDidFinishLoading:(NSURLConnection*) myConnection {
		UNUSED ( myConnection );
		self->mTask->DidFinishLoading ();
	}



@end
