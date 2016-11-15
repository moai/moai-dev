// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLRESULT_H
#define ZLRESULT_H

#include <zl-util/STLString.h>

#define ZL_RETURN_RESULT(type,value,code) {		\
	ZLResult < type > __result;					\
	__result.mValue = value;					\
	__result.mCode = code;						\
	return __result;							\
}

#define	ZL_RETURN_BOOL_RESULT(value,code)		ZL_RETURN_RESULT ( bool, value, code )
#define	ZL_RETURN_CHAR_RESULT(value,code)		ZL_RETURN_RESULT ( u8, value, code )
#define	ZL_RETURN_DOUBLE_RESULT(value,code)		ZL_RETURN_RESULT ( double, value, code )
#define	ZL_RETURN_FLOAT_RESULT(value,code)		ZL_RETURN_RESULT ( float, value, code )
#define	ZL_RETURN_INT_RESULT(value,code)		ZL_RETURN_RESULT ( int, value, code )
#define	ZL_RETURN_SIZE_RESULT(value,code)		ZL_RETURN_RESULT ( size_t, value, code )
#define	ZL_RETURN_STRING_RESULT(value,code)		ZL_RETURN_RESULT ( STLString, value, code )

#define ZL_HANDLE_ERROR_CODE(code,handler) {	\
	ZLResultCode CODE = code;					\
	if ( CODE != ZL_OK ) {						\
		handler;								\
	}											\
}

#define ZL_HANDLE_ERROR_RESULT(result,handler)	ZL_HANDLE_ERROR_CODE ( result.mCode, handler )

#define ZL_RETURN_IF_NOT_EQUAL(result,expected,type,value,code) {					\
	ZLResultCode CODE = result.mCode;												\
	if ( CODE != ZL_OK ) ZL_RETURN_RESULT ( type, value, CODE )						\
	if ( result.mValue != expected ) ZL_RETURN_RESULT ( type, value, code )			\
}

#define	ZL_RETURN_BOOL_RESULT_IF_NOT_EQUAL(result,expected,value,code)		ZL_RETURN_IF_NOT_EQUAL ( result, expected, bool, value, code )
#define	ZL_RETURN_CHAR_RESULT_IF_NOT_EQUAL(result,expected,value,code)		ZL_RETURN_IF_NOT_EQUAL ( result, expected, u8, value, code )
#define	ZL_RETURN_DOUBLE_RESULT_IF_NOT_EQUAL(result,expected,value,code)	ZL_RETURN_IF_NOT_EQUAL ( result, expected, double, value, code )
#define	ZL_RETURN_FLOAT_RESULT_IF_NOT_EQUAL(result,expected,value,code)		ZL_RETURN_IF_NOT_EQUAL ( result, expected, float, value, code )
#define	ZL_RETURN_INT_RESULT_IF_NOT_EQUAL(result,expected,value,code)		ZL_RETURN_IF_NOT_EQUAL ( result, expected, int, value, code )
#define	ZL_RETURN_SIZE_RESULT_IF_NOT_EQUAL(result,expected,value,code)		ZL_RETURN_IF_NOT_EQUAL ( result, expected, size_t, value, code )
#define	ZL_RETURN_STRING_RESULT_IF_NOT_EQUAL(result,expected,value,code)	ZL_RETURN_IF_NOT_EQUAL ( result, expected, STLString, value, code )

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
	
	//----------------------------------------------------------------//
	inline ZLResultCode Code () const {
		return this->mCode;
	}
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
	
	//----------------------------------------------------------------//
	inline TYPE Value () const {
		return this->mValue;
	}
};

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

typedef	ZLResult < bool >		ZLBoolResult;
typedef	ZLResult < u8 >			ZLCharResult;
typedef	ZLResult < double >		ZLDoubleResult;
typedef	ZLResult < float >		ZLFloatResult;
typedef	ZLResult < int >		ZLIntResult;
typedef	ZLResult < size_t >		ZLSizeResult;
typedef	ZLResult < STLString >	ZLStringResult;

#endif
