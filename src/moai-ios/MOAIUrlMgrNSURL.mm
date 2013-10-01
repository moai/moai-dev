// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-iphone/MOAIUrlMgrNSURL.h>

//----------------------------------------------------------------//
void MOAIUrlMgrNSURL::AddHandle ( MOAIHttpTaskNSURL& task ) {
	
	NSURLConnection* handle = task.mEasyHandle;
	if ( !handle ) return;
	
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
	
	task->FinishRequest ();
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
	
	if ( task != NULL ) {

		task->mResponseCode = responseCode;
		task->mExpectedLength = expectedLength;
		
		for ( id key in headers ) {
			task->mResponseHeaders [ [key UTF8String] ] = [[ headers objectForKey:key ] UTF8String ];
		}
	}
}


