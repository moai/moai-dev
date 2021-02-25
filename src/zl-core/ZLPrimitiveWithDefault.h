// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLPRIMTIVEWITHDEFAULT_H
#define	ZLPRIMTIVEWITHDEFAULT_H

//================================================================//
// ZLPrimitiveWithDefault
//================================================================//
template < typename TYPE, TYPE DEFAULT_VALUE = 0 >
class ZLPrimitiveWithDefault {
private:

	TYPE mValue;

public:

	//----------------------------------------------------------------//
	operator TYPE& () {
		return this->mValue;
	}
	
	//----------------------------------------------------------------//
	operator const TYPE& () const {
		return this->mValue;
	}

	//----------------------------------------------------------------//
	ZLPrimitiveWithDefault () :
		mValue ( DEFAULT_VALUE ) {
	}

	//----------------------------------------------------------------//
	ZLPrimitiveWithDefault ( const TYPE& value ) :
		mValue ( value ) {
	}
};

#endif
