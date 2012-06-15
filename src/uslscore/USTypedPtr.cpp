// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USTypedPtr.h>

//================================================================//
// USTypedPtr
//================================================================//

//----------------------------------------------------------------//
bool USTypedPtr::IsMatch ( USTypedPtr& src ) {
	return ( this->mTypeID == src.mTypeID );
}

//----------------------------------------------------------------//
void USTypedPtr::Nullify () {
	this->mData = 0;
}

//----------------------------------------------------------------//
USTypedPtr::USTypedPtr () :
	mData ( 0 ) {
}
