//
//  MOAIUrlMgrNSURL.h
//  libmoai
//
//  Created by Megan Sullivan on 7/31/12.
//
//

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
	void			ProcessResponse (NSURLConnection* handle, int responseCode) ;
	void			ConnectionDidFinishLoading (NSURLConnection* handle);

};




#endif
