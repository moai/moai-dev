// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCALLBACKWITHUSERDATA_H
#define ZLCALLBACKWITHUSERDATA_H

//================================================================//
// ZLCallbackWithUserdata
//================================================================//
template < typename FUNC_TYPE >
class ZLCallbackWithUserdata {
public:

	FUNC_TYPE	mFunc;
	void*		mUserdata;

	//----------------------------------------------------------------//
	inline operator bool () const {
		return ( this->mFunc != NULL );
	};

	//----------------------------------------------------------------//
	ZLCallbackWithUserdata () :
		mFunc ( NULL ),
		mUserdata ( NULL ) {
	}
	
	//----------------------------------------------------------------//
	ZLCallbackWithUserdata ( FUNC_TYPE func, void* userdata) :
		mFunc ( func ),
		mUserdata ( userdata ) {
	}
	
	//----------------------------------------------------------------//
	~ZLCallbackWithUserdata () {
	}
};

#endif
