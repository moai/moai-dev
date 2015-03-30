// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIJSTRING_H
#define	MOAIJSTRING_H

#include <jni.h>

#include <moai-android/MOAIJString.h>
#include <moai-android/JniUtils.h>


class MOAIJString {
private:

	jstring			mJString;
		
public:

	//----------------------------------------------------------------//
	inline void operator = ( jstring& assign ) {

		this->mJString = assign;
	};

	//----------------------------------------------------------------//
	inline void operator = ( const jstring& assign ) {

		this->mJString = ( jstring )assign;
	};

	//----------------------------------------------------------------//
	inline operator jstring () {
		return this->mJString;
	};

	//----------------------------------------------------------------//
	inline operator const jstring () const {
		return this->mJString;
	};

	//----------------------------------------------------------------//
	MOAIJString () :
		mJString ( 0 ) {
	}

	//----------------------------------------------------------------//
	MOAIJString ( const jstring& assign ) :
		mJString (( jstring )assign ) {
	}

	//----------------------------------------------------------------//
	MOAIJString ( jstring& assign ) :
		mJString ( assign ) {
	}

	//----------------------------------------------------------------//
	~MOAIJString ();
};

#endif

