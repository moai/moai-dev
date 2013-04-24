// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef libmoai_MOAIUrlMgrNSURL_h
#define libmoai_MOAIUrlMgrNSURL_h


//#ifdef USE_CURL

#include <moaiext-iphone/MOAIHttpTaskNSURL.h>

//================================================================//
// MOAIUrlMgrCurl
//================================================================//
class MOAIUrlMgrNSURL :
public MOAIGlobalClass < MOAIUrlMgrNSURL > {
private:
	
	STLMap < NSURLConnection*, MOAIHttpTaskNSURL* > mHandleMap;
	//CURLM*	mMultiHandle;
	bool	mMore;
	
	//----------------------------------------------------------------//
	void			AddHandle				( MOAIHttpTaskNSURL& task );
	
public:
	
	friend class MOAIHttpTaskNSURL;
	
	//----------------------------------------------------------------//
	MOAIUrlMgrNSURL			();
	~MOAIUrlMgrNSURL			();
	void			Process					(NSURLConnection* handle, const void* data, int size);
	void			ProcessResponse (NSURLConnection* handle, int responseCode, NSDictionary* headers, int expectedLength ) ;
	void			ConnectionDidFinishLoading (NSURLConnection* handle);

};




#endif
