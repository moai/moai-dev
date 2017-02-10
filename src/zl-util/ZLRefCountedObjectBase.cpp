// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLRefCountedObjectBase.h>

//================================================================//
// ZLRefCountedObjectBase
//================================================================//

//----------------------------------------------------------------//
void ZLRelease ( ZLRefCountedObjectBase* object ) {

	if ( object ) {
		object->Release ();
	}
}

//----------------------------------------------------------------//
void ZLRetain ( ZLRefCountedObjectBase* object ) {

	if ( object ) {
		object->Retain ();
	}
}
