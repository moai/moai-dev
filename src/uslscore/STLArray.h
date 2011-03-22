// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef XVEC_H
#define	XVEC_H

#include <vector>

using namespace std;

//================================================================//
// STLArray
//================================================================//
/**	@brief Extends STL 'vector' to add... nothing so far.
	
	@tparam TYPE The type of element this vector will hold.
*/
template < typename TYPE >
class STLArray :
	public vector < TYPE > {
public:
};

#endif
