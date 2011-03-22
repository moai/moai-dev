// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslsext/USAttributed.h>
#include <uslsext/USAttrMap.h>

//================================================================//
// USAttrMap
//================================================================//

//----------------------------------------------------------------//
void USAttrMap::Clear () {

	this->mMap.clear ();
}

//----------------------------------------------------------------//
void USAttrMap::MapAttr ( u32 primeID, u32 subID ) {

	this->mMap [ primeID ] = subID;
}

//----------------------------------------------------------------//
void USAttrMap::ReadAttributes ( USAttributed& prime, USAttributed& sub ) {

	USAttrGetter getter;
	USAttrSetter setter;

	FOREACH ( MapIt, mapIt, this->mMap ) {
		
		sub.ApplyAttrOp ( mapIt->second, getter );
		setter.Copy ( getter );
		prime.ApplyAttrOp ( mapIt->first, setter );
	}
}

//----------------------------------------------------------------//
USAttrMap::USAttrMap () {
}

//----------------------------------------------------------------//
USAttrMap::~USAttrMap () {
}

//----------------------------------------------------------------//
void USAttrMap::WriteAttributes ( USAttributed& prime, USAttributed& sub ) {

	USAttrGetter getter;
	USAttrSetter setter;

	FOREACH ( MapIt, mapIt, this->mMap ) {
		
		prime.ApplyAttrOp ( mapIt->first, getter );
		setter.Copy ( getter );
		sub.ApplyAttrOp ( mapIt->second, setter );
	}
}
