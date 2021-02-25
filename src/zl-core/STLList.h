// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef STLLIST_H
#define	STLLIST_H

#include <list>

//================================================================//
// STLList
//================================================================//
template < typename TYPE >
class STLList :
	public std::list < TYPE > {
public:

	typedef typename std::list < TYPE >::iterator iterator;
	typedef typename std::list < TYPE >::reverse_iterator reverse_iterator;

	//----------------------------------------------------------------//
	bool contains ( const TYPE& type ) {
	
		iterator it = this->begin ();
		for ( ; it != this->end (); ++it ) {
			if (( *it ) == type ) return true;
		}
		return false;
	}

	//----------------------------------------------------------------//
	void copy ( STLList < TYPE >& list ) {
	
		this->clear ();
		
		iterator it = list.begin ();
		for ( ; it != list.end (); ++it ) {
			this->push_back ( *it );
		}
	}

	//----------------------------------------------------------------//
	void erase_first ( const TYPE& type ) {
	
		iterator it = this->begin ();
		for ( ; it != this->end (); ++it ) {
			if (( *it ) == type ) {
				this->erase ( it );
				return;
			}
		}
	}

	//----------------------------------------------------------------//
	void erase_last ( const TYPE& type ) {
		
		reverse_iterator it = this->rbegin ();
		for ( ; it != this->rend (); ++it ) {
			if (( *it ) == type ) {
				this->erase ( it );
				return;
			}
		}
	}

	//----------------------------------------------------------------//
	iterator find_first ( const TYPE& type ) {
	
		iterator it = this->begin ();
		for ( ; it != this->end (); ++it ) {
			if (( *it ) == type ) return it;
		}
		return this->end ();
	}

	//----------------------------------------------------------------//
	iterator find_last ( const TYPE& type ) {
		
		iterator result = this->end ();
		iterator it = this->begin ();
		for ( ; it != this->end (); ++it ) {
			if (( *it ) == type ) result = it;
		}
		return result;
	}

	//----------------------------------------------------------------//
	TYPE& extend_back () {
		this->push_back ( TYPE ());
		return this->back ();
	}
	
	//----------------------------------------------------------------//
	TYPE& extend_front () {
		this->push_front ( TYPE ());
		return this->front ();
	}
	
	//----------------------------------------------------------------//
	void remove_all ( const TYPE& type ) {
	
		iterator it = this->begin ();
		while ( it != this->end ()) {
			iterator eraseIt = it;
			++it;
			
			if ( *eraseIt == type ) {
				this->erase ( eraseIt );
			}
		}
	}
	
	//----------------------------------------------------------------//
	void remove_first ( const TYPE& type ) {
	
		iterator it = this->begin ();
		while ( it != this->end ()) {
			if ( *it == type ) {
				this->erase ( it );
				return;
			}
			++it;
		}
	}
	
	//----------------------------------------------------------------//
	void remove_last ( const TYPE& type ) {
	
		reverse_iterator it = this->rbegin ();
		while ( it != this->rend ()) {
			if ( *it == type ) {
				this->erase ( it );
				return;
			}
			++it;
		}
	}
};

#endif
