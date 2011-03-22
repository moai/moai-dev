// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USAttributed.h>

//================================================================//
// USAttributed
//================================================================//

//----------------------------------------------------------------//
void USAttributed::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {
	UNUSED ( attrID );
	UNUSED ( attrOp );
}

//----------------------------------------------------------------//
bool USAttributed::AttrExists ( u32 attrID ) {

	if ( attrID == NULL_ATTR ) return false;

	USAttrGetter getter;
	this->ApplyAttrOp ( attrID, getter );
	return getter.IsValid ();
}

//----------------------------------------------------------------//
u32 USAttributed::CountAttributes () {

	return 0;
}

//----------------------------------------------------------------//
USAttributed::USAttributed ()  {
}

//----------------------------------------------------------------//
USAttributed::~USAttributed ()  {
}
