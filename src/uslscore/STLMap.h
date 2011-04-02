// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef XMAP_H
#define	XMAP_H

#include <map>

using namespace std;

//================================================================//
// STLMap
//================================================================//
template < typename KEY, typename TYPE >
class STLMap :
	public map < KEY, TYPE > {
public:

	typedef typename map < KEY, TYPE >::iterator iterator;

	//----------------------------------------------------------------//
	bool contains ( const KEY& key ) {
		return ( this->find ( key ) != this->end ());
	}

	//----------------------------------------------------------------//
	TYPE* key_for_value ( const KEY& key ) {
	
		iterator it = this->find ( key );
		if ( it != this->end ()) {
			return &it->second;
		}
		return 0;
	}
};

//================================================================//
// STLMap < TYPE* >
//================================================================//
template < typename KEY, typename TYPE >
class STLMap < KEY, TYPE* > :
	public map < KEY, TYPE* > {
public:

	typedef typename map < KEY, TYPE* >::iterator iterator;

	//----------------------------------------------------------------//
	bool contains ( const KEY& key ) {
		return ( this->find ( key ) != this->end ());
	}

	//----------------------------------------------------------------//
	TYPE* value_for_key ( const KEY& key ) {
	
		iterator it = this->find ( key );
		if ( it != this->end ()) {
			return it->second;
		}
		return 0;
	}
};

#endif
