// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLRESULT_H
#define ZLRESULT_H

#include <zl-util/STLString.h>

enum {
	ZL_OK,
	ZL_ERROR,
	ZL_ALLOCATION_ERROR,
	ZL_RANGE_ERROR,
	ZL_UNSUPPORTED,
	ZL_TOTAL_ERRORS,
};

typedef int ZLResultCode;

//================================================================//
// ZLResultBase
//================================================================//
class ZLResultBase {
public:

	ZLResultCode	mCode;
	
//	//----------------------------------------------------------------//
//	inline ZLResultCode Code () {
//		return this->mCode;
//	}
};

//================================================================//
// ZLResult
//================================================================//
template < typename TYPE >
class ZLResult :
	public ZLResultBase {
public:

	TYPE	mValue;
	
	//----------------------------------------------------------------//
	inline operator const TYPE& () const {
		assert ( this->mCode == ZL_OK );
		return this->mValue;
	}
};

typedef ZLResult < bool >		ZLBoolResult;
typedef ZLResult < double >		ZLDoubleResult;
typedef ZLResult < float >		ZLFloatResult;
typedef ZLResult < int >		ZLIntResult;
typedef ZLResult < size_t >		ZLSizeResult;
typedef ZLResult < STLString >	ZLStringResult;

//================================================================//
// ZLResultCodeAccumulator
//================================================================//
class ZLResultCodeAccumulator {
private:

	ZLResultCode	mCode;

public:

	//----------------------------------------------------------------//
	inline operator const int () const {
		return this->mCode;
	}
	
	//----------------------------------------------------------------//
	inline void operator = ( int code ) {
		this->mCode = code != ZL_OK ? code : this->mCode;
	}
	
	//----------------------------------------------------------------//
	inline int Reset () {
		int code = this->mCode;
		this->mCode = ZL_OK;
		return code;
	}
	
	//----------------------------------------------------------------//
	ZLResultCodeAccumulator () :
		mCode ( ZL_OK ) {
	}
};

#define ZL_RETURN_RESULT(type,value,code) {		\
	type __result;								\
	__result.mValue = value;					\
	__result.mCode = code;						\
	return __result;							\
}

#define ZL_RETURN_BOOL_RESULT(value,code)		ZL_RETURN_RESULT ( ZLBoolResult, value, code )
#define ZL_RETURN_DOUBLE_RESULT(value,code)		ZL_RETURN_RESULT ( ZLDoubleResult, value, code )
#define ZL_RETURN_FLOAT_RESULT(value,code)		ZL_RETURN_RESULT ( ZLFloatResult, value, code )
#define ZL_RETURN_INT_RESULT(value,code)		ZL_RETURN_RESULT ( ZLIntResult, value, code )
#define ZL_RETURN_SIZE_RESULT(value,code)		ZL_RETURN_RESULT ( ZLSizeResult, value, code )
#define ZL_RETURN_STRING_RESULT(value,code)		ZL_RETURN_RESULT ( ZLStringResult, value, code )

#endif
