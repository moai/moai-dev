// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLCLEANUP_H
#define ZLCLEANUP_H

//================================================================//
// ZLCleanup
//================================================================//
template < typename TYPE >
class ZLCleanup {
protected:

	typedef void ( TYPE::*SelectorType )();

	SelectorType	mSelector; // NOTE: this member *must* be first in the class (vs2015)
	TYPE*			mTarget;
	
public:

	//----------------------------------------------------------------//
	virtual void Skip () {
		this->mTarget			= 0;
		this->mSelector			= 0;
	}

	//----------------------------------------------------------------//
	ZLCleanup ( TYPE* target, SelectorType selector ) :
		mTarget ( target ),
		mSelector ( selector ) {
		
		assert ( target );
		assert ( selector );
	}

	//----------------------------------------------------------------//
	~ZLCleanup () {
	
		if ( this->mTarget && this->mSelector ) {
			( this->mTarget->*this->mSelector )();
		}
	}
};

#endif
