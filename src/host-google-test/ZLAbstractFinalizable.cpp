// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "ZLAbstractFinalizable.h"

//================================================================//
// ZLAbstractFinalizable
//================================================================//
	
//----------------------------------------------------------------//
ZLAbstractFinalizable::ZLAbstractFinalizable () {
}

//----------------------------------------------------------------//
ZLAbstractFinalizable::~ZLAbstractFinalizable () {
}

//----------------------------------------------------------------//
void ZLAbstractFinalizable::Finalize () {
	void* self = this->ZLAbstractFinalizable_GetSelf ();
	this->ZLAbstractFinalizable_Finalize ( self );
}
