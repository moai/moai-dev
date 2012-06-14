// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USTYPEDPTR_H
#define	USTYPEDPTR_H

#include <uslscore/USTypeID.h>

//================================================================//
// USTypedPtr
//================================================================//
class USTypedPtr {
private:

	u32		mTypeID;
	void*	mData;

public:

	//----------------------------------------------------------------//
	bool	IsMatch		( USTypedPtr& src );
			USTypedPtr	();
	void	Nullify		();

	//----------------------------------------------------------------//
	inline operator bool () {
		return ( mData != 0 );
	};
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	inline operator TYPE () {
		return this->Get < TYPE >();
	}

	//----------------------------------------------------------------//
	template < typename	TYPE >
	TYPE* Get () {
		if ( this->mData )
			if ( this->mTypeID == USTypeID < TYPE >::GetID ())
				return ( TYPE* )this->mData;
		return 0;
	}

	//----------------------------------------------------------------//
	template < typename	TYPE >
	bool IsType () {
		return ( this->mTypeID == USTypeID < TYPE >::GetID ());
	}

	//----------------------------------------------------------------//
	template < typename	TYPE >
	void Set ( TYPE* data ) {
		mTypeID = USTypeID < TYPE >::GetID ();
		mData = data;
	}
};

#endif
