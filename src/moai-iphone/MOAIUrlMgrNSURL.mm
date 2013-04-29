// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-iphone/MOAIUrlMgrNSURL.h>

void MOAIUrlMgrNSURL::AddHandle ( MOAIHttpTaskNSURL& task ) {
	
	NSURLConnection* handle = task.mEasyHandle;
	if ( !handle ) return;
	
	//USLog::Print("AddHandle %i\n", handle);
	
	task.Retain ();
	task.LockToRefCount ();
	
	//curl_multi_add_handle ( this->mMultiHandle, handle );
	this->mHandleMap [ handle ] = &task;
	this->mMore = true;
}

//----------------------------------------------------------------//
MOAIUrlMgrNSURL::MOAIUrlMgrNSURL () :
//mMultiHandle ( 0 ),
mMore ( false ) {
	
	//this->mMultiHandle = curl_multi_init ();
}

//----------------------------------------------------------------//
MOAIUrlMgrNSURL::~MOAIUrlMgrNSURL () {
	
	//if ( this->mMultiHandle ) {
	//	curl_multi_cleanup ( this->mMultiHandle );
	//}
}


void MOAIUrlMgrNSURL::ConnectionDidFinishLoading (NSURLConnection* handle)
{
	
	
	STLMap < NSURLConnection*, MOAIHttpTaskNSURL* >& handleMap = this->mHandleMap;
	
	MOAIHttpTaskNSURL* task = handleMap [ handle ];
	handleMap.erase ( handle );
	
	//USLog::Print("finish data for handle %i\n", handle);
	
	task->CurlFinish ();
	task->Release ();
	
}


//----------------------------------------------------------------//
void MOAIUrlMgrNSURL::Process (NSURLConnection* handle, const void* data, int size) {
	
	STLMap < NSURLConnection*, MOAIHttpTaskNSURL* >& handleMap = this->mHandleMap;
	//CURLM* multiHandle = this->mMultiHandle;
	
	//if ( !this->mMore ) return;
	//this->mMore = false;
	
	//if ( !multiHandle ) return;
	
	// pump the multi handle
	//int stillRunning;
	//while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform ( multiHandle, &stillRunning ));
	
	//int msgsInQueue;
	//CURLMsg* msg;
	//do {
		
	//	msg = curl_multi_info_read ( multiHandle, &msgsInQueue );
		
	//	if ( msg && ( msg->msg == CURLMSG_DONE )) {
			
	//USLog::Print("Process for handle %i\n", handle);
	//		CURL* handle = msg->easy_handle;
	//		if ( handleMap.contains ( handle )) {
	MOAIHttpTaskNSURL* task = handleMap [ handle ];
			
	task->mDataReceived += size;
	task->mProgress = task->mDataReceived / task->mExpectedLength;
	MOAIHttpTaskNSURL::_writeData((char*)data, size, 1, task);

	//		}
	//	}
	//}
	//while ( msg );
	
	// bail if nothing left running
	//if ( !stillRunning ) return;
	
	//this->mMore = true;
}

void MOAIUrlMgrNSURL::ProcessResponse (NSURLConnection* handle, int responseCode, NSDictionary* headers, int expectedLength ) {
	
	STLMap < NSURLConnection*, MOAIHttpTaskNSURL* >& handleMap = this->mHandleMap;
	
	MOAIHttpTaskNSURL* task = handleMap [ handle ];
	//handleMap.erase ( handle );
	
	if (task == NULL)
	{
		//USLog::Print ( "MOAIUrlMgrNSURL::ProcessResponse Error getting handle %i\n", handle);
		
	}
	else
	{
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


