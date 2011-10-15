// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USAttributed.h>

//================================================================//
// USAttributed
//================================================================//

//----------------------------------------------------------------//
bool USAttributed::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp, u32 op ) {
	UNUSED ( attrID );
	UNUSED ( attrOp );
	UNUSED ( op );
	
	return false;
}

//----------------------------------------------------------------//
bool USAttributed::AttrExists ( u32 attrID ) {

	if ( attrID == NULL_ATTR ) return false;

	USAttrOp getter;
	this->ApplyAttrOp ( attrID, getter, USAttrOp::CHECK );
	return getter.IsValid ();
}

//----------------------------------------------------------------//
USAttributed::USAttributed ()  {
}

//----------------------------------------------------------------//
USAttributed::~USAttributed ()  {
}
