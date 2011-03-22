// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USData.h>

//================================================================//
// USData
//================================================================//

//----------------------------------------------------------------//
void USData::Clear () {

	this->mMutex.Lock ();
	this->mBytes.Clear ();
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
bool USData::Load ( cc8* filename ) {

	USFileStream in;
	if ( !in.OpenRead ( filename )) return false;

	this->mMutex.Lock ();

	u32 size = in.GetLength ();
	this->mBytes.Init ( size );
	in.ReadBytes ( this->mBytes , size );

	this->mMutex.Unlock ();

	return true;
}

//----------------------------------------------------------------//
void USData::Load ( void* bytes, u32 size ) {

	this->mMutex.Lock ();
	
	this->mBytes.Init ( size );
	memcpy ( this->mBytes.Data (), bytes, size );
	
	this->Unlock ();
}

//----------------------------------------------------------------//
void USData::Lock ( void** bytes, u32* size ) {

	this->mMutex.Lock ();
	( *bytes ) = this->mBytes;
	( *size ) = this->mBytes.Size ();
}

//----------------------------------------------------------------//
bool USData::Save ( cc8* filename, bool affirm_path ) {

	USFileStream out;

	if ( !out.OpenWrite ( filename, affirm_path ) ) return false;

	this->mMutex.Lock ();
	out.WriteBytes ( this->mBytes , this->mBytes.Size ());
	this->mMutex.Unlock ();

	return true;
}

//----------------------------------------------------------------//
void USData::Unlock () {

	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
USData::USData () {
}

//----------------------------------------------------------------//
USData::~USData () {

	this->Clear ();
}
