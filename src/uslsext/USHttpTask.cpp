// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslsext/USDataIOTask.h>
#include <uslsext/USFileCache.h>
#include <uslsext/USHttpTask.h>
#include <uslsext/USHttpTask_impl.h>
#include <uslsext/USUrlMgr.h>

//================================================================//
// USHttpTask
//================================================================//

//----------------------------------------------------------------//
void USHttpTask::CacheFile () {

	if ( !this->mStoreKey.size ()) return;
	if ( !this->mFileCache ) return;
	if ( !this->mFileCache->IsConnected ()) return;

	this->Retain ();
	this->mDataFromCache.Load ( this->mBytes, this->mSize );
	
	USTaskThread& taskThread = USUrlMgr::Get ().mDataIOThread;
	USDataIOTask* task = taskThread.NewTask < USDataIOTask >();

	STLString filePath = this->mFileCache->GetFilePath ( this->mStoreKey );

	task->SaveData ( filePath, this->mDataFromCache );
	task->SetDelegate ( this, &USHttpTask::CacheFileFinish );
}

//----------------------------------------------------------------//
void USHttpTask::CacheFileFinish ( USDataIOTask* task ) {
	UNUSED ( task );

	void* bytes;
	u32 size;
	this->mDataFromCache.Lock ( &bytes, &size );

	if ( this->mFileCache->IsConnected ()) {
		this->mFileCache->AffirmFileEntry (
			this->mStoreKey,
			size,
			this->mCachePriority
		);
	}

	this->mDataFromCache.Unlock ();
	
	this->Release ();
}

//----------------------------------------------------------------//
void USHttpTask::Cancel () {

	this->Clear ();
	this->mCallback.Clear ();
	//this->Release ();
	//this->Finish ();
}

//----------------------------------------------------------------//
void USHttpTask::Clear () {

	if ( this->mInfo ) {
		delete this->mInfo;
		this->mInfo = 0;
	}
	
	this->mBytes = 0;
	this->mSize = 0;
}

//----------------------------------------------------------------//
void USHttpTask::Finish () {

	this->mInfo->Finish ();
	
	this->mBytes = this->mInfo->mData;
	this->mSize = this->mInfo->mData.Size ();

	this->CacheFile ();

	this->mCallback.Call ( this );

	this->Clear ();
	this->Release ();
}

//----------------------------------------------------------------//
void* USHttpTask::GetData () {

	return this->mBytes;
}

//----------------------------------------------------------------//
void USHttpTask::GetData ( void* buffer, u32 size ) {

	USByteStream byteStream;
	
	byteStream.SetBuffer ( this->mBytes, this->mSize );
	byteStream.ReadBytes ( buffer, size );
}

//----------------------------------------------------------------//
u32 USHttpTask::GetSize () {

	return this->mSize;
}

//----------------------------------------------------------------//
void USHttpTask::LoadFile ( cc8* filename ) {

	this->Retain ();

	USTaskThread& taskThread = USUrlMgr::Get ().mDataIOThread;
	USDataIOTask* task = taskThread.NewTask < USDataIOTask >();

	task->LoadData ( filename, this->mDataFromCache );
	task->SetDelegate ( this, &USHttpTask::LoadFileFinish );
}

//----------------------------------------------------------------//
void USHttpTask::LoadFileFinish ( USDataIOTask* task ) {
	UNUSED ( task );

	this->mDataFromCache.Lock ( &this->mBytes, &this->mSize );
	this->mCallback.Call ( this );
	this->mDataFromCache.Unlock ();
	
	this->Clear ();
	this->Release ();
}

//----------------------------------------------------------------//
void USHttpTask::HttpGet ( cc8* url ) {

	this->Clear ();
	this->mInfo = new USHttpTaskInfo ();
	this->mInfo->InitForGet ( url );
	
	this->Retain ();
	USUrlMgr::Get ().AddHandle ( *this );
}

//----------------------------------------------------------------//
void USHttpTask::HttpGet ( cc8* url, cc8* storeKey ) {

	this->Clear ();
	this->mStoreKey = storeKey;
	
	bool fetch = true;
	
	STLString localPath;
	
	if ( this->mFileCache && storeKey ) {
		localPath = this->mFileCache->GetFileName ( storeKey );
		
	}
	
	if ( localPath.size ()) {
		this->LoadFile ( localPath );
		fetch = false;
	}
	else if ( fetch ) {
		this->HttpGet ( url );
	}
}

//----------------------------------------------------------------//
void USHttpTask::HttpPost ( cc8* url, const void* buffer, u32 size ) {

	this->Clear ();
	this->mInfo = new USHttpTaskInfo ();
	this->mInfo->InitForPost ( url, buffer, size );
	
	this->Retain ();
	USUrlMgr::Get ().AddHandle ( *this );
}

//----------------------------------------------------------------//
void USHttpTask::SetFileCache ( USFileCache* fileCache, int cachePriority ) {

	this->mFileCache = fileCache;
	this->mCachePriority = cachePriority;
}

//----------------------------------------------------------------//
USHttpTask::USHttpTask () :
	mInfo ( 0 ),
	mFileCache ( 0 ),
	mCachePriority ( 0 ),
	mBytes ( 0 ),
	mSize ( 0 ) {
}

//----------------------------------------------------------------//
USHttpTask::USHttpTask ( const USHttpTask& task ) {
	UNUSED ( task );
	assert ( false ); // copy constructor not supported
}

//----------------------------------------------------------------//
USHttpTask::~USHttpTask () {

	this->Clear ();
}
