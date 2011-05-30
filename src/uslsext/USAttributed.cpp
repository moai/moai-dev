// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USAttributed.h>

//================================================================//
// USAttributed
//================================================================//

//----------------------------------------------------------------//
bool USAttributed::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {
	UNUSED ( attrID );
	UNUSED ( attrOp );
	
	return false;
}

//----------------------------------------------------------------//
bool USAttributed::AttrExists ( u32 attrID ) {

	if ( attrID == NULL_ATTR ) return false;

	USAttrGetter getter;
	this->ApplyAttrOp ( attrID, getter );
	return getter.IsValid ();
}

//----------------------------------------------------------------//
USAttributed::USAttributed ()  {
}

//----------------------------------------------------------------//
USAttributed::~USAttributed ()  {
}
