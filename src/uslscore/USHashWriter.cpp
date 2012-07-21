// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USHashWriter.h>

//================================================================//
// USHashWriter
//================================================================//


//----------------------------------------------------------------//
void USHashWriter::Close () {

	if ( this->mIsOpen ) {
		this->FinalizeHash ();	
	}
	this->mOutputStream = 0;
	this->mIsOpen = false;
}

//----------------------------------------------------------------//
u32 USHashWriter::GetCaps () {
	
	return this->mOutputStream ? CAN_WRITE : 0;
}

//----------------------------------------------------------------//
size_t USHashWriter::GetCursor () {
	
	return 0;
}

//----------------------------------------------------------------//
size_t USHashWriter::GetLength () {
	
	return 0;
}

//----------------------------------------------------------------//
bool USHashWriter::Open ( USStream* stream ) {
	
	this->Close ();
	this->mOutputStream = stream;
	this->InitHash ();
	this->mIsOpen = true;
	return true;
}

//----------------------------------------------------------------//
USHashWriter::USHashWriter () :
	mIsOpen ( false ),
	mOutputStream ( 0 ) {
}

//----------------------------------------------------------------//
USHashWriter::~USHashWriter () {
}

//----------------------------------------------------------------//
size_t USHashWriter::WriteBytes ( const void* buffer, size_t size ) {
	
	// Pass the write through to stream
	if ( this->mOutputStream ) {
		size = this->mOutputStream->WriteBytes( buffer, size );
	}
	
	// Update the hash
	if ( size ) {
		this->HashBytes ( buffer, size );
	}
	
	return size;
}
