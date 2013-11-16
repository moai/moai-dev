// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <iostream>

#include "pch.h"

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
	
	//this->mEasyHandle = 1;
	/*
	if ( this->mEasyHandle ) return;
	
	CURLcode result;
	
	this->mEasyHandle = curl_easy_init ();
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_HEADERFUNCTION, _writeHeader );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_HEADERDATA, this );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_WRITEFUNCTION, _writeData );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_WRITEDATA, this );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_FAILONERROR, 1 );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_NOPROGRESS, 1 );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_SSL_VERIFYPEER, 0 );
	PrintError ( result );
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_SSL_VERIFYHOST, 2 );
	PrintError ( result );
	 */
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
	//	curl_easy_cleanup ( this->mEasyHandle );
		this->mEasyHandle = 0;
	}
	
	//if ( this->mHeaderList ) {
	//	curl_slist_free_all ( this->mHeaderList );
	//	this->mHeaderList = 0;
	//}
	 
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::CurlFinish () {
	/*
	if ( this->mEasyHandle ) {
		long response;
		curl_easy_getinfo ( this->mEasyHandle, CURLINFO_RESPONSE_CODE, &response );
		this->mResponseCode = ( u32 )response;
	}
	*/
	
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
mEasyHandle ( 0 ),
//mHeaderList ( 0 ,
mDataReceived ( 0 ),
mStream ( 0 )
{
	
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
	
	
	//USLog::Print("Prepare %s\n", "for transfer");
	// until we get a header indicating otherwise, assume we won't
	// know the final length of the stream, so default to use the
	// USMemStream which will grow dynamically
	if ( this->mUserStream ) {
		
		this->mStream = this->mUserStream->GetZLStream();
	}
	else {
		
		this->mStream = &this->mMemStream;
	}
	/*
	char buffer [ MAX_HEADER_LENGTH ];
	
	// prepare the custom headers (if any)
	HeaderMapIt headerMapIt = this->mHeaderMap.begin ();
	for ( ; headerMapIt != this->mHeaderMap.end (); ++headerMapIt ) {
		
		STLString key = headerMapIt->first;
		STLString value = headerMapIt->second;
		
		assert (( key.size () + value.size () + 3 ) < MAX_HEADER_LENGTH );
		
		if ( value.size ()) {
			sprintf ( buffer, "%s: %s", key.c_str (), value.c_str ());
		}
		else {
			sprintf ( buffer, "%s:", key.c_str ());
		}
		
		this->mHeaderList = curl_slist_append ( this->mHeaderList, buffer );
	}
	
	if ( this->mHeaderList ) {
		CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_HTTPHEADER, this->mHeaderList );
		PrintError ( result );
	}
	
	CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CONNECTTIMEOUT, this->mDefaultTimeout );
	
	// follow redirects based on settings in base class (default is to NOT follow redirects)
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_FOLLOWLOCATION, this->mFollowRedirects );
	
	// set the timeout for this task
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_TIMEOUT, this->mTimeout );
	
	PrintError ( result );
	 */
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::PerformAsync () {
	
	this->mEasyHandle = [NSURLConnection alloc];
	
	if ( this->mEasyHandle ) {
		this->Prepare ();
		MOAIUrlMgrNSURL::Get ().AddHandle ( *this );
	}

	NSString *requestString = [NSString stringWithCString:mUrl encoding:NSUTF8StringEncoding];
	//USLog::Print("PerformAsync URL %s\n", [requestString UTF8String]);
	NSURL * myURL = [[NSURL alloc] initWithString:requestString];
	NSMutableURLRequest *myRequest = [NSMutableURLRequest requestWithURL: myURL
															 cachePolicy: NSURLRequestReloadIgnoringLocalCacheData
														 timeoutInterval: mTimeout];
	switch ( this->mOpt ) {
				
		case HTTP_GET:
				
			[myRequest setHTTPMethod:@"GET" ];
			break;
				
		case HTTP_HEAD:
			[myRequest setHTTPMethod:@"HEAD" ];
			break;
				
		case HTTP_POST:
			[myRequest setHTTPMethod:@"POST" ];
			break;
				
		case HTTP_PUT:
			[myRequest setHTTPMethod:@"PUT" ];
			break;
				
		case HTTP_DELETE:
			[myRequest setHTTPMethod:@"DELETE" ];
			break;
	}
	
	
	if ( this->mHeaderMap.size() > 0 ) {
		
		for ( auto& pair: this->mHeaderMap ) {
			
			//printf ( "%s, %s\n", pair.first.c_str(), pair.second.c_str());
			NSString* value = [[ NSString alloc ] initWithUTF8String:pair.second];
			NSString* key = [[ NSString alloc ] initWithUTF8String:pair.first];
			[myRequest addValue:value forHTTPHeaderField:key];
		}
	}
	
	if (mBody.Size() > 0)
	{
		//USLog::Print ( "mBody.Size() %i\n", mBody.Size());
		
		[myRequest setHTTPBody:[NSData dataWithBytes:mBody.Data() length:mBody.Size()]];
		mBody.Clear();
	}

	[myURL release];
	
	dispatch_async(dispatch_get_main_queue(), ^{
		connection_ = [this->mEasyHandle initWithRequest: myRequest delegate: mUrlDelegate];
	});
	
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::PerformSync () {

	
	NSString *requestString = [NSString stringWithCString:mUrl encoding:NSUTF8StringEncoding];
	//USLog::Print("PerformSync URL %s\n", [requestString UTF8String]);
	NSURL * myURL = [[NSURL alloc] initWithString:requestString];
	NSMutableURLRequest *myRequest = [NSMutableURLRequest requestWithURL: myURL
															 cachePolicy: NSURLRequestReloadIgnoringLocalCacheData
														 timeoutInterval: mTimeout];
	
	switch ( this->mOpt ) {
			
		case HTTP_GET:
			
			[myRequest setHTTPMethod:@"GET" ];
			break;
			
		case HTTP_HEAD:
			[myRequest setHTTPMethod:@"HEAD" ];
			break;
			
		case HTTP_POST:
			[myRequest setHTTPMethod:@"POST" ];
			break;
			
		case HTTP_PUT:
			[myRequest setHTTPMethod:@"PUT" ];
			break;
			
		case HTTP_DELETE:
			[myRequest setHTTPMethod:@"DELETE" ];
			break;
	}
	
	
	if ( this->mHeaderMap.size() > 0 ) {
		
		for ( auto& pair: this->mHeaderMap ) {
			
			NSString* value = [[ NSString alloc ] initWithUTF8String:pair.second];
			NSString* key = [[ NSString alloc ] initWithUTF8String:pair.first];
			[myRequest addValue:value forHTTPHeaderField:key];
		}
	}
	
	if (mBody.Size() > 0)
	{
		//USLog::Print ( "mBody.Size() %i\n", mBody.Size());
		
		[myRequest setHTTPBody:[NSData dataWithBytes:mBody.Data() length:mBody.Size()]];
		mBody.Clear();
	}
	
	[myURL release];
	
	//connection_ = [this->mEasyHandle initWithRequest: myRequest delegate: mUrlDelegate];
	NSHTTPURLResponse* myResponse = nil;
	NSError* myError = nil;
	
	
	this->Prepare ();

	NSData* data = [NSURLConnection sendSynchronousRequest:myRequest returningResponse:&myResponse error:&myError];
	
    this->mResponseCode = [myResponse statusCode];
	
	if (myError == NULL)
	{
		//[this->mUrlDelegate connection:this->mEasyHandle didReceiveResponse:response];
		
		if (data)
		{
			_writeData((char*)data.bytes, data.length, 1, this);
		}
	}
	else
	{
		NSLog(@"Error: %@ %@", myError, [myError userInfo]);
	}
	
	this->CurlFinish ();

	
}

//----------------------------------------------------------------//
//void MOAIHttpTaskNSURL::PrintError ( CURLcode error ) {
//
//	if ( error ) {
//		USLog::Print ( "%s\n", curl_easy_strerror ( error ));
//	}
//}

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

	//USLog::Print ( "setBody %i\n", size);
	
	this->mBody.Init ( size );
	memcpy ( this->mBody, buffer, size );
	
	/*
	CURLcode result;
	
	result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_POSTFIELDS, this->mBody.Data ());
	PrintError ( result );
	
    result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_POSTFIELDSIZE, ( long )size );
    PrintError ( result );
	 */
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetFailOnError ( bool enable ) {
	
	
	UNUSED(enable);
	//CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_FAILONERROR, ( u32 ) enable );
	//PrintError ( result );
	
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetCookieDst	( const char *file ) {
	
	UNUSED(file);
	/*
	CURLcode result = curl_easy_setopt( this->mEasyHandle, CURLOPT_COOKIEFILE, file );
	PrintError ( result );
	*/
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetCookieSrc	( const char *file ) {
	
	UNUSED(file);
	/*
	CURLcode result = curl_easy_setopt( this->mEasyHandle, CURLOPT_COOKIEJAR, file );
	PrintError ( result );
	 */
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetUrl ( cc8* url ) {

	//CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_URL, url );
	//PrintError ( result );
	
	this->mUrl = url;
	
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetUserAgent ( cc8* useragent ) {
	
	UNUSED(useragent);
	/*
	CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_USERAGENT, useragent );
	PrintError ( result );
	 */
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetVerb ( int verb ) {
	

	//printf ( "%d\n", verb );
	this->mOpt = verb;
	//CURLcode result = CURLE_OK;
	
	/*switch ( verb ) {
			
		case HTTP_GET:
			//result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CUSTOMREQUEST, "GET" );
			this->mOpt = "GET";
			break;
			
		case HTTP_HEAD:
			//result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CUSTOMREQUEST, "HEAD" );
			this->mOpt = "HEAD";
			break;
			
		case HTTP_POST:
			//result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CUSTOMREQUEST, "POST" );
			this->mOpt = "POST";
			break;
			
		case HTTP_PUT:
			//result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CUSTOMREQUEST, "PUT" );
			this->mOpt = "PUT";
			break;
			
		case HTTP_DELETE:
			//result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_CUSTOMREQUEST, "DELETE" );
			this->mOpt = "DELETE";
			break;
	}*/
	
	//PrintError ( result );
	
	//result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_NOBODY, verb == HTTP_HEAD ? 1 : 0 );
	//PrintError ( result );
	
}

//----------------------------------------------------------------//
void MOAIHttpTaskNSURL::SetVerbose ( bool verbose ) {
	
	UNUSED(verbose);
	/*
	CURLcode result = curl_easy_setopt ( this->mEasyHandle, CURLOPT_VERBOSE, verbose ? 1 : 0 );
	PrintError ( result );
	 */
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

- (BOOL)connection:(NSURLConnection *)connection canAuthenticateAgainstProtectionSpace:(NSURLProtectionSpace *)protectionSpace {
	
	UNUSED(connection);
	
		//USLog::Print ( "%s\n", "canAuthenticateAgainstProtectionSpace for nsurl");
	return [protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust];
}

- (void)connection:(NSURLConnection *)connection didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge {
	
	UNUSED(connection);
	//if ([challenge.protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust])
	//	if (... user allows connection despite bad certificate ...)
			[challenge.sender useCredential:[NSURLCredential credentialForTrust:challenge.protectionSpace.serverTrust] forAuthenticationChallenge:challenge];
	
	//USLog::Print ( "%s\n", "didReceiveAuthenticationChallenge for nsurl");
	
	[challenge.sender continueWithoutCredentialForAuthenticationChallenge:challenge];
}



	- (void)connection:(NSURLConnection*) myConnection didReceiveResponse:(NSURLResponse*) myResponse;
	{
		
		//if ( this->mEasyHandle ) {
		//	long response;
		//	curl_easy_getinfo ( this->mEasyHandle, CURLINFO_RESPONSE_CODE, &response );
	//		this->mResponseCode = [myResponse statusCode];
	//	}
		
		//this->mResponseCode = [myResponse statusCode];
		
		NSDictionary* headers = [(NSHTTPURLResponse *)myResponse allHeaderFields];
		int theCount = 0;
		theCount = [ headers count ];
		//printf ( " header count %d\n", theCount );
		MOAIUrlMgrNSURL::Get ().ProcessResponse(myConnection, [(NSHTTPURLResponse*)myResponse statusCode], headers, (float)[(NSHTTPURLResponse*)myResponse expectedContentLength]);
		
		//MOAIHttpTaskNSURL::Get ().didReceiveResponse ( [myResponse statusCode]);
		
		//MOAIUrlMgrNSURL::Get ().didReceiveResponse ( [myResponse statusCode] );
		
		//USLog::Print ( "%s\n", "response received for nsurl");
	}


	- (void)connection:(NSURLConnection*) myConnection didReceiveData:(NSData*) myData;
	{

		MOAIUrlMgrNSURL::Get ().Process(myConnection, myData.bytes, myData.length);
		
		//USLog::Print ( "%s\n", "data received for nsurl");
	}


	- (void)connection:(NSURLConnection*) myConnection didFailWithError:(NSError*) myError;
	{
		//USLog::Print ( "%s\n", "data received data with error nsurl");
		
		NSLog(@"Error: %@ %@", myError, [myError userInfo]);
		MOAIUrlMgrNSURL::Get ().ConnectionDidFinishLoading(myConnection);
	}


	- (void)connectionDidFinishLoading:(NSURLConnection*) myConnection;
	{
		//[connection_ release];
		//connection_ = nil;
		
		MOAIUrlMgrNSURL::Get ().ConnectionDidFinishLoading(myConnection);
		
		//USLog::Print ( "%s\n", "connections loaded nsurl");
		
	}



@end






//#endif
