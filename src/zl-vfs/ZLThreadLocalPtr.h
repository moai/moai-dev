// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLTHREADLOCALPTR_H
#define ZLTHREADLOCALPTR_H

//================================================================//
// ZLThreadLocalBase
//================================================================//
class ZLThreadLocalBase {
protected:

	void*	mKey;

	//----------------------------------------------------------------//
	void*		GetPtr					();
	void		SetPtr					( void* value );
				ZLThreadLocalBase		();
				~ZLThreadLocalBase		();
};

//================================================================//
// ZLThreadLocal
//================================================================//
template < typename TYPE >
class ZLThreadLocalPtr :
	protected ZLThreadLocalBase {
public:

	//----------------------------------------------------------------//
	TYPE* Get () {
		return ( TYPE* )this->GetPtr ();
	}

	//----------------------------------------------------------------//
	void Set ( TYPE* assign ) {
		this->SetPtr ( assign );
	}
};

#endif
