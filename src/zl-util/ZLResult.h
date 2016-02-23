// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLRESULT_H
#define ZLRESULT_H

#include <zl-util/STLString.h>

//================================================================//
// ZLResultBase
//================================================================//
class ZLResultBase {
protected:

	int		mCode;

public:
	
	//----------------------------------------------------------------//
	inline int Code () {
		return this->mCode;
	}
};

//================================================================//
// ZLResultCode
//================================================================//
class ZLResultCode :
	public ZLResultBase {
public:
	
	//----------------------------------------------------------------//
	ZLResultCode ( int code ) {
		this->mCode = code;
	}
};

//================================================================//
// ZLResult
//================================================================//
template < typename TYPE >
class ZLResult :
	public ZLResultBase {
private:

	TYPE mValue;

public:
	
	//----------------------------------------------------------------//
	inline operator const TYPE& () const {
		assert ( !this->mCode );
		return this->mValue;
	}
	
	//----------------------------------------------------------------//
	inline const TYPE& Value () {
		assert ( !this->mCode );
		return this->mValue;
	}
	
	//----------------------------------------------------------------//
	ZLResult ( const TYPE& value )  {
		this->mValue = value;
		this->mCode = 0;
	}
	
	//----------------------------------------------------------------//
	ZLResult ( const TYPE& value, int code ) {
		this->mValue = value;
		this->mCode = code;
	}
};

typedef ZLResult < bool > ZLBoolResult;
typedef ZLResult < double > ZLDoubleResult;
typedef ZLResult < float > ZLFloatResult;
typedef ZLResult < int > ZLIntResult;
typedef ZLResult < size_t > ZLSizeResult;
typedef ZLResult < STLString > ZLStringResult;

#endif
