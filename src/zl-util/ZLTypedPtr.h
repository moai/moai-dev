// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTYPEDPTR_H
#define	ZLTYPEDPTR_H

#include <zl-util/ZLTypeID.h>

//================================================================//
// ZLTypedPtr
//================================================================//
class ZLTypedPtr {
private:

	u32		mTypeID;
	void*	mData;

public:

	//----------------------------------------------------------------//
	bool	IsMatch		( ZLTypedPtr& src );
	void	Nullify		();
			ZLTypedPtr	();

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
			if ( this->mTypeID == ZLTypeID < TYPE >::GetID ())
				return ( TYPE* )this->mData;
		return 0;
	}

	//----------------------------------------------------------------//
	template < typename	TYPE >
	bool IsType () {
		return ( this->mTypeID == ZLTypeID < TYPE >::GetID ());
	}

	//----------------------------------------------------------------//
	template < typename	TYPE >
	void Set ( TYPE* data ) {
		mTypeID = ZLTypeID < TYPE >::GetID ();
		mData = data;
	}
};

#endif
