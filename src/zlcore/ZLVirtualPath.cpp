// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zlcore/zl_util.h>
#include <zlcore/ZLVirtualPath.h>

using namespace std;

//================================================================//
// ZLVirtualPath
//================================================================//

//----------------------------------------------------------------//
const char* ZLVirtualPath::GetLocalPath ( const char* path ) {

	if ( this->mArchive ) {
		
		size_t baselen = this->mPath.size ();

		if ( strlen ( path ) <= baselen ) return "";

		path = &path [ baselen ];
		
		if ( this->mArchive->FindDir ( path )) return path;
	}
	return 0;
}

//----------------------------------------------------------------//
ZLVirtualPath* ZLVirtualPath::PushFront ( ZLVirtualPath* list ) {
	
	this->mNext = list;
	return this;
}

//----------------------------------------------------------------//
int ZLVirtualPath::SetArchive ( const char* archive ) {
	
	if ( this->mArchive ) {
		delete this->mArchive;
	}
	
	this->mArchive = new ZLZipArchive ();
	int result = this->mArchive->Open ( archive );
	if ( result ) {
		delete this->mArchive;
		this->mArchive = 0;
		return -1;
	}
	
	return 0;
}

//----------------------------------------------------------------//
int ZLVirtualPath::SetPath ( const char* path ) {

	size_t base = 0;
	size_t i = 0;
	size_t namelen = 0;

	this->mPath = path;
	
	for ( ; path [ i ]; ++i ) {
		if (( path [ i ] == '/' ) && path [ i + 1 ]) {
			base = i + 1;
		}
	}
	
	namelen = i - base;
	if ( path [ i - 1 ] == '/' ) namelen--;

	this->mName = this->mPath.substr ( base, namelen );

	return 0;
}

//----------------------------------------------------------------//
ZLVirtualPath::ZLVirtualPath () :
	mArchive ( 0 ) {
}

//----------------------------------------------------------------//
ZLVirtualPath::~ZLVirtualPath () {

	if ( this->mArchive ) {
		delete this->mArchive;
	}
}
