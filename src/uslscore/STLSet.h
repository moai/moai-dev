// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef XSET_H
#define	XSET_H

#include <set>

using namespace std;

//================================================================//
// STLSet
//================================================================//
/**	@brief Extends STL 'set' to add some helper functions.
	
	@tparam KEY The type of key this set uses.
*/
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
