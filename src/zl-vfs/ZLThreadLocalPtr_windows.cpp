// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai_config.h>
#include <zl-vfs/ZLThreadLocalPtr.h>

#ifdef _WIN32

#include <windows.h>

//================================================================//
// ZLThreadLocalBase
//================================================================//

//----------------------------------------------------------------//
void* ZLThreadLocalBase::GetPtr () {

	return TlsGetValue ( *( DWORD* )this->mKey );
}

//----------------------------------------------------------------//
void ZLThreadLocalBase::SetPtr ( void* value ) {

	TlsSetValue ( *( DWORD* )this->mKey, value );
}

//----------------------------------------------------------------//
ZLThreadLocalBase::ZLThreadLocalBase () {

	this->mKey = malloc ( sizeof DWORD );
	*( DWORD* )this->mKey = TlsAlloc ();
}

//----------------------------------------------------------------//
ZLThreadLocalBase::~ZLThreadLocalBase () {

	TlsFree ( *( DWORD* )this->mKey );
	free ( this->mKey );
}

#endif