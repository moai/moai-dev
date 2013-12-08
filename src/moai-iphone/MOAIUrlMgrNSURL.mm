// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-iphone/MOAIUrlMgrNSURL.h>

//----------------------------------------------------------------//
void MOAIUrlMgrNSURL::AddHandle ( MOAIHttpTaskNSURL& task ) {
	
	NSURLConnection* handle = task.mEasyHandle;
	if ( !handle ) return;
	
	//USLog::Print("AddHandle %i\n", handle);
	
	task.LatchRetain ();
	
	this->mHandleMap [ handle ] = &task;
	this->mMore = true;
}

//----------------------------------------------------------------//
void MOAIUrlMgrNSURL::ConnectionDidFinishLoading (NSURLConnection* handle)
{
	
	
	STLMap < NSURLConnection*, MOAIHttpTaskNSURL* >& handleMap = this->mHandleMap;
	
	MOAIHttpTaskNSURL* task = handleMap [ handle ];
	handleMap.erase ( handle );
	
	//USLog::Print("finish data for handle %i\n", handle);
	
	task->CurlFinish ();
	task->LatchRelease ();
}

//----------------------------------------------------------------//
MOAIUrlMgrNSURL::MOAIUrlMgrNSURL () :
	mMore ( false ) {
}

//----------------------------------------------------------------//
MOAIUrlMgrNSURL::~MOAIUrlMgrNSURL () {
}

//----------------------------------------------------------------//
void MOAIUrlMgrNSURL::Process (NSURLConnection* handle, const void* data, int size) {
	
	STLMap < NSURLConnection*, MOAIHttpTaskNSURL* >& handleMap = this->mHandleMap;
	
	MOAIHttpTaskNSURL* task = handleMap [ handle ];
			
	task->mDataReceived += size;
	task->mProgress = task->mDataReceived / task->mExpectedLength;
	MOAIHttpTaskNSURL::_writeData((char*)data, size, 1, task);
}

//----------------------------------------------------------------//
void MOAIUrlMgrNSURL::ProcessResponse ( NSURLConnection* handle, int responseCode, NSDictionary* headers, int expectedLength ) {
	
	STLMap < NSURLConnection*, MOAIHttpTaskNSURL* >& handleMap = this->mHandleMap;
	
	MOAIHttpTaskNSURL* task = handleMap [ handle ];
	//handleMap.erase ( handle );
	
	if ( task == NULL ) {
		//USLog::Print ( "MOAIUrlMgrNSURL::ProcessResponse Error getting handle %i\n", handle);
		
	}
	else {
		//USLog::Print ( "MOAIUrlMgrNSURL::ProcessResponse Response Code %i handle \n", responseCode, handle);
		task->mResponseCode = responseCode;
		task->mExpectedLength = expectedLength;
		
		//printf ( "header count %d\n", [headers count]);
		for ( id key in headers ) {
			//printf ( "^^^^^^^^^^^ %s, %s\n", [key UTF8String],[[headers objectForKey:key] UTF8String] );
			task->mResponseHeaders [ [key UTF8String] ] = [[headers objectForKey:key] UTF8String];
		}
	}
	//MOAIHttpTaskNSURL::_writeData((char*)data, size, 1, task);
}


