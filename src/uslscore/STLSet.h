// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef XSET_H
#define	XSET_H

#include <set>

using namespace std;

//================================================================//
// STLSet
//================================================================//
template < typename KEY >
class STLSet :
	public set < KEY > {
public:

	//----------------------------------------------------------------//
	bool affirm ( const KEY& key ) {
	
		if ( !this->contains ( key )) {
			this->insert ( key );
			return true;
		}
		return false;
	}

	//----------------------------------------------------------------//
	bool contains ( const KEY& key ) {
		return ( this->find ( key ) != this->end ());
	}
};

#endif
