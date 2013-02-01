// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USBase64Reader.h>
#include <uslscore/USBase64Writer.h>
#include <uslscore/USByteStream.h>
#include <uslscore/USData.h>
#include <uslscore/USDeflateReader.h>
#include <uslscore/USDeflateWriter.h>
#include <uslscore/USFileStream.h>
#include <uslscore/USHexReader.h>
#include <uslscore/USHexWriter.h>
#include <uslscore/USMemStream.h>
#include <uslscore/USStreamReader.h>
#include <uslscore/USStreamWriter.h>

//================================================================//
// USData
//================================================================//

//----------------------------------------------------------------//
bool USData::Base64Decode () {

	USBase64Reader base64;
	return this->Decode ( base64 );
}

//----------------------------------------------------------------//
bool USData::Base64Encode () {

	USBase64Writer base64;
	return this->Encode ( base64 );
}

//----------------------------------------------------------------//
void USData::Clear () {

	this->mMutex.Lock ();
	this->mBytes.Clear ();
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
bool USData::Decode ( USStreamReader& reader ) {
	
	this->mMutex.Lock ();
	
	USByteStream cryptStream;
	cryptStream.SetBuffer ( this->mBytes, this->mBytes.Size ());
	cryptStream.SetLength ( this->mBytes.Size ());
	
	USMemStream plainStream;
	
	reader.Open ( &cryptStream );
	plainStream.WriteStream ( reader );
	reader.Close ();
	
	size_t len = plainStream.GetLength ();
	this->mBytes.Init ( len );
	
	plainStream.Seek ( 0, SEEK_SET );
	plainStream.ReadBytes ( this->mBytes, len );
	
	this->mMutex.Unlock ();
	return true;
}

//----------------------------------------------------------------//
bool USData::Deflate ( int level, int windowBits ) {

	USDeflateWriter deflater;
	deflater.SetCompressionLevel ( level );
	deflater.SetWindowBits ( windowBits );
	
	return this->Encode ( deflater );
}

//----------------------------------------------------------------//
bool USData::Encode ( USStreamWriter& writer ) {
	
	this->mMutex.Lock ();
	
	USMemStream stream;
	
	writer.Open ( &stream );
	writer.WriteBytes ( this->mBytes, this->mBytes.Size ());
	writer.Close ();
	
	size_t len = stream.GetLength ();
	this->mBytes.Init ( len );
	
	stream.Seek ( 0, SEEK_SET );
	stream.ReadBytes ( this->mBytes, len );

	this->mMutex.Unlock ();
	return true;
}

//----------------------------------------------------------------//
bool USData::HexDecode () {

	USHexReader hex;
	return this->Decode ( hex );
}

//----------------------------------------------------------------//
bool USData::HexEncode () {

	USHexWriter hex;
	return this->Encode ( hex );
}

//----------------------------------------------------------------//
bool USData::Inflate ( int windowBits ) {

	USDeflateReader inflater;
	inflater.SetWindowBits ( windowBits );
	
	return this->Decode ( inflater );
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
void USData::Load ( void* bytes, size_t size ) {

	this->mMutex.Lock ();
	
	this->mBytes.Init ( size );
	memcpy ( this->mBytes.Data (), bytes, size );
	
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
void USData::Lock ( void** bytes, size_t* size ) {

	this->mMutex.Lock ();
	( *bytes ) = this->mBytes;
	( *size ) = this->mBytes.Size ();
}

//----------------------------------------------------------------//
bool USData::Save ( cc8* filename ) {

	USFileStream out;

	if ( !out.OpenWrite ( filename )) return false;

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
