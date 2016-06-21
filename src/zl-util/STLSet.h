// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef STLSET_H
#define	STLSET_H

#include <set>

//================================================================//
// STLSet
//================================================================//
template < typename KEY, typename COMPARE = std::less < KEY > >
class STLSet :
	public std::set < KEY, COMPARE > {
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
	bool contains ( const KEY& key ) const {
		return ( this->find ( key ) != this->end ());
	}
};

#endif
