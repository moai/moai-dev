// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <iostream>

#include <moai-http-client/pch.h>
#include <algorithm>
#include <moai-iphone/MOAIUrlMgrNSURL.h>
#include <moai-iphone/MOAIHttpTaskNSURL.h>

SUPPRESS_EMPTY_FILE_WARNING
//#ifdef USE_NSURL

#define MAX_HEADER_LENGTH 1024


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

//----------------------------------------------------------------//
u32 MOAIHttpTaskNSURL::_writeHeader ( char* data, u32 n, u32 l, void* s ) {
	
	UNUSED(data);
	UNUSED(n);
	UNUSED(l);
	UNUSED(s);
	
	/*
	MOAIHttpTaskNSURL* self = ( MOAIHttpTaskNSURL* )s;
	u32 size = n * l;
	
	char *endp = data + size;
	char *colon = data;
	while ( colon < endp && *colon != ':' ) {
		colon++;
	}
	
	if ( colon < endp )
	{
		STLString name ( data, colon - data );
		// Case insensitive
		
		char *vstart = colon;
		vstart++;
		while( vstart < endp && isspace ( *vstart )) {
			vstart++;
		}
		char *vend = endp - 1;
		while( vend > vstart && isspace ( *vend ) ) {
			vend--;
		}
		STLString value(vstart, ( vend - vstart ) + 1);
		
		// Emulate XMLHTTPRequest.getResponseHeader() behavior of appending with comma
		// separator if there are multiple header responses?
		
		if( self->mResponseHeaders.find ( name ) != self->mResponseHeaders.end ())	{
			self->mResponseHeaders [ name ] = self->mResponseHeaders [ name ] + "," + value;
		}
		else {
			self->mResponseHeaders [ name ] = value;
		}
	}
	
	// Shouldn't this be a case-insensitive check?
	
	STLString key = "content-length";
	u32 keyLength = ( u32 )strlen ( key );
	if ( strncmp ( data, key, keyLength ) == 0 ) {
		
		STLString header = data;
		u32 end = ( u32 )header.find_last_of ( '\n' );
		STLString value = header.clip ( keyLength + 2, end - 1 );
		
		u32 length = atoi ( value );
		if ( length ) {
			
			self->mData.Init ( length );
			self->mByteStream.SetBuffer ( self->mData, length );
			self->mByteStream.SetLength ( length );
			self->mStream = &self->mByteStream;
		}
	}
	return size;
	 
	 */
	return 0;
}

//================================================================//
// MOAIHttpTaskNSURL
//================================================================//

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::AffirmHandle () {
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::Clear () {
	
	this->mUrl.clear ();
	this->mBody.Clear ();
	this->mMemStream.Clear ();
	this->mData.Clear ();
	this->mResponseHeaders.clear();
	
	this->mResponseCode = 0;
	this->mStream = 0;
	
	if ( this->mEasyHandle ) {
		this->mEasyHandle = 0;
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
	
	[ this->mEasyHandle release ];
	this->mEasyHandle = 0;
	
	[ this->mRequest release ];
	this->mRequest = 0;
	
	this->Finish ();
	
}

//----------------------------------------------------------------//
MOAIHttpTaskNSURL::MOAIHttpTaskNSURL () :
	mOpt ( 0 ),
	mDefaultTimeout ( 10 ),
	mEasyHandle ( 0 ),
	mDataReceived ( 0 ),
	mStream ( 0 ),
	mRequest ( 0 ) {
	
	RTTI_SINGLE ( MOAIHttpTaskBase )

	this->Reset ();

	this->mOpt = 0;
	mUrlDelegate = [ MOAIHttpTaskNSURLDelegate alloc ];

}

//----------------------------------------------------------------//
MOAIHttpTaskNSURL::~MOAIHttpTaskNSURL () {
	
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::Prepare () {
	
	// until we get a header indicating otherwise, assume we won't
	// know the final length of the stream, so default to use the
	// USMemStream which will grow dynamically
	this->mStream = this->mUserStream ? this->mUserStream->GetZLStream () : &this->mMemStream;
	
	NSString* requestString = [ NSString stringWithCString:mUrl encoding:NSUTF8StringEncoding ];
	NSURL* myURL = [ NSURL URLWithString:requestString ];
	this->mRequest = [[ NSMutableURLRequest requestWithURL: myURL cachePolicy: NSURLRequestReloadIgnoringLocalCacheData timeoutInterval: mTimeout ] retain ];
	
	switch ( this->mOpt ) {
				
		case HTTP_GET:
			[ this->mRequest setHTTPMethod:@"GET" ];
			break;
				
		case HTTP_HEAD:
			[ this->mRequest setHTTPMethod:@"HEAD" ];
			break;
				
		case HTTP_POST:
			[ this->mRequest setHTTPMethod:@"POST" ];
			break;
				
		case HTTP_PUT:
			[ this->mRequest setHTTPMethod:@"PUT" ];
			break;
				
		case HTTP_DELETE:
			[ this->mRequest setHTTPMethod:@"DELETE" ];
			break;
	}
	
	if ( this->mHeaderMap.size() > 0 ) {
		for ( auto& pair: this->mHeaderMap ) {
			NSString* value = [ NSString stringWithUTF8String:pair.second ];
			NSString* key = [ NSString stringWithUTF8String:pair.first ];
			[ this->mRequest addValue:value forHTTPHeaderField:key ];
		}
	}
	
	if ( mBody.Size () > 0 ) {
		[ this->mRequest setHTTPBody:[ NSData dataWithBytes:mBody.Data () length:mBody.Size ()]];
		mBody.Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::PerformAsync () {
	
	this->mEasyHandle = [[ NSURLConnection alloc ] init ];
	MOAIUrlMgrNSURL::Get ().AddHandle ( *this );

	this->Prepare ();
	
	dispatch_async ( dispatch_get_main_queue(), ^{ connection_ = [ this->mEasyHandle initWithRequest: this->mRequest delegate: mUrlDelegate ]; });
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::PerformSync () {

	this->Prepare ();
	
	NSHTTPURLResponse* myResponse = nil;
	NSError* myError = nil;
	
	this->Prepare ();

	NSData* data = [ NSURLConnection sendSynchronousRequest:this->mRequest returningResponse:&myResponse error:&myError ];
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
	
	mBody.Clear();
	
	this->Clear ();
	this->AffirmHandle ();
	 
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
	- (BOOL)connection:(NSURLConnection *)connection canAuthenticateAgainstProtectionSpace:(NSURLProtectionSpace *)protectionSpace {
		
		UNUSED(connection);
		
		return [protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust];
	}

	//----------------------------------------------------------------//
	- (void)connection:(NSURLConnection *)connection didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge {
		
		UNUSED(connection);
		//if ([challenge.protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust])
		//	if (... user allows connection despite bad certificate ...)
				[challenge.sender useCredential:[NSURLCredential credentialForTrust:challenge.protectionSpace.serverTrust] forAuthenticationChallenge:challenge];
		
		[challenge.sender continueWithoutCredentialForAuthenticationChallenge:challenge];
	}

	//----------------------------------------------------------------//
	- (void)connection:(NSURLConnection*) myConnection didReceiveResponse:(NSURLResponse*) myResponse {
	
		NSDictionary* headers = [(NSHTTPURLResponse *)myResponse allHeaderFields];
		int theCount = 0;
		theCount = [ headers count ];
		MOAIUrlMgrNSURL::Get ().ProcessResponse(myConnection, [(NSHTTPURLResponse*)myResponse statusCode], headers, (float)[(NSHTTPURLResponse*)myResponse expectedContentLength]);
	}

	//----------------------------------------------------------------//
	- (void)connection:(NSURLConnection*) myConnection didReceiveData:(NSData*) myData {
	
		MOAIUrlMgrNSURL::Get ().Process(myConnection, myData.bytes, myData.length);
	}

	//----------------------------------------------------------------//
	- (void)connection:(NSURLConnection*) myConnection didFailWithError:(NSError*) myError {
	
		NSLog ( @"Error: %@ %@", myError, [ myError userInfo ]);
		MOAIUrlMgrNSURL::Get ().ConnectionDidFinishLoading ( myConnection );
	}

	//----------------------------------------------------------------//
	- (void)connectionDidFinishLoading:(NSURLConnection*) myConnection {
	
		MOAIUrlMgrNSURL::Get ().ConnectionDidFinishLoading ( myConnection );
	}



@end






//#endif
