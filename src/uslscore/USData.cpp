// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USBase64Cipher.h>
#include <uslscore/USByteStream.h>
#include <uslscore/USCipher.h>
#include <uslscore/USCipherStream.h>
#include <uslscore/USData.h>
#include <uslscore/USDeflater.h>
#include <uslscore/USFileStream.h>
#include <uslscore/USInflater.h>
#include <uslscore/USMemStream.h>
#include <uslscore/USStreamFormatter.h>

//================================================================//
// USData
//================================================================//

//----------------------------------------------------------------//
bool USData::Base64Decode () {

	USBase64Cipher base64;
	return this->Decode ( base64 );
}

//----------------------------------------------------------------//
bool USData::Base64Encode () {

	USBase64Cipher base64;
	return this->Encode ( base64 );
}

//----------------------------------------------------------------//
void USData::Clear () {

	this->mMutex.Lock ();
	this->mBytes.Clear ();
	this->mMutex.Unlock ();
}

//----------------------------------------------------------------//
bool USData::Decode ( USCipher& cipher ) {
	
	this->mMutex.Lock ();
	
	USByteStream cryptStream;
	cryptStream.SetBuffer ( this->mBytes, this->mBytes.Size ());
	cryptStream.SetLength ( this->mBytes.Size ());
	
	USCipherStream cipherStream;
	cipherStream.OpenCipher ( cryptStream, cipher );
	
	USMemStream plainStream;
	plainStream.Pipe ( cipherStream );
	
	cipherStream.CloseCipher ();
	
	size_t len = plainStream.GetLength ();
	this->mBytes.Init ( len );
	
	plainStream.Seek ( 0, SEEK_SET );
	plainStream.ReadBytes ( this->mBytes, len );
	
	this->mMutex.Unlock ();
	return true;
}

//----------------------------------------------------------------//
bool USData::Deflate ( int level, int windowBits ) {

	USDeflater deflater;
	deflater.SetCompressionLevel ( level );
	deflater.SetWindowBits ( windowBits );
	
	return this->Transform ( deflater );
}

//----------------------------------------------------------------//
bool USData::Encode ( USCipher& cipher ) {
	
	this->mMutex.Lock ();
	
	USCipherStream cipherStream;
	USMemStream stream;
	
	cipherStream.OpenCipher ( stream, cipher );
	cipherStream.WriteBytes ( this->mBytes, this->mBytes.Size ());
	cipherStream.CloseCipher ();
	
	size_t len = stream.GetLength ();
	this->mBytes.Init ( len );
	
	stream.Seek ( 0, SEEK_SET );
	stream.ReadBytes ( this->mBytes, len );

	this->mMutex.Unlock ();
	return true;
}

//----------------------------------------------------------------//
bool USData::Inflate ( int windowBits ) {

	USInflater inflater;
	inflater.SetWindowBits ( windowBits );
	
	return this->Transform ( inflater );
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
bool USData::Save ( cc8* filename, bool affirm_path ) {

	USFileStream out;

	if ( !out.OpenWrite ( filename, affirm_path ) ) return false;

	this->mMutex.Lock ();
	out.WriteBytes ( this->mBytes , this->mBytes.Size ());
	this->mMutex.Unlock ();

	return true;
}

//----------------------------------------------------------------//
bool USData::Transform ( USStreamFormatter& formatter ) {

	this->mMutex.Lock ();
	
	USMemStream stream;
	
	formatter.SetStream ( &stream );
	formatter.WriteBytes ( this->mBytes, this->mBytes.Size ());
	formatter.Flush ();
	
	size_t len = stream.GetLength ();
	this->mBytes.Init ( len );
	
	stream.Seek ( 0, SEEK_SET );
	stream.ReadBytes ( this->mBytes, len );
	
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
