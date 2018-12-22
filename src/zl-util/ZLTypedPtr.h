// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTYPEDPTR_H
#define	ZLTYPEDPTR_H

#include <zl-util/ZLType.h>

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
	inline operator bool () const {
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
			if ( this->mTypeID == ZLType::GetID < TYPE >())
				return ( TYPE* )this->mData;
		return 0;
	}

	//----------------------------------------------------------------//
	template < typename	TYPE >
	bool IsType () {
		return ( this->mTypeID == ZLType::GetID < TYPE >());
	}

	//----------------------------------------------------------------//
	template < typename	TYPE >
	void Set ( TYPE* data ) {
		mTypeID = ZLType::GetID < TYPE >();
		mData = data;
	}
};

#endif
