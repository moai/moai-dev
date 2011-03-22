// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef XMAP_H
#define	XMAP_H

#include <map>

using namespace std;

//================================================================//
// STLMap
//================================================================//
/**	@brief Extends STL 'map' to add some helper functions.
	
	@tparam KEY The type of key this map uses.
	@tparam TYPE The type of value this map uses.
*/
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
/**	@brief Specializes STLMap for pointer value types.
	
	@tparam KEY The type of key this map uses.
	@tparam TYPE The type of value this map uses.
*/
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
