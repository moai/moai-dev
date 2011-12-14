#include <cstdio>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

#include "ppapi/cpp/file_ref.h"

#include "NaClFileSystem.h"
#include "moai_nacl.h"

#include "geturl_handler.h"

//AJV TODO - this needs to be refactored, will be switched to caching files
// as needed, check disk first then download.
// auto-caching will double disk space for packages apps though...

NaClFileSystem *NaClFileSystem::mSingletonInstance = NULL;

//----------------------------------------------------------------//
NaClFileSystem::~NaClFileSystem () {
	mCore = NULL;
	mInstance = NULL;
}

//----------------------------------------------------------------//
NaClFileSystem::NaClFileSystem ( pp::Core* core, pp::Instance *instance ) :
	mCCFactory ( this ),
	mFileSystem ( instance, PP_FILESYSTEMTYPE_LOCALPERSISTENT )
{
	mCore = core;
	mInstance = instance;
	mSingletonInstance = this;
	mFileSystemOpened = false;
}

//----------------------------------------------------------------//
void NaClFileSystem::Init () {

	pp::CompletionCallback cc ( OpenFileSystemMainThread, this );
	mCore->CallOnMainThread ( 0, cc , 0 );

	while ( !mFileSystemOpened ) {

		usleep ( 1000 );
	}
}

//----------------------------------------------------------------//
void NaClFileSystem::OpenFileSystemMainThread ( void* userData, int32_t result ) {

	pp::CompletionCallback cc = Get ()->mCCFactory.NewCallback ( &NaClFileSystem::OpenFileSystemCallback );

	int32_t res = Get ()->mFileSystem.Open ( 1024, cc );

	if ( PP_OK_COMPLETIONPENDING != res ) {
		cc.Run ( res );
	}
}

//----------------------------------------------------------------//
void NaClFileSystem::OpenFileSystemCallback ( int32_t result ) {

	if ( result != PP_OK ) {
		NACL_LOG ( "ERROR: NaClFileSystem Open File system result %d\n", result );

		//AJV TODO send message back to browser
		g_instance->PostMessage ( pp::Var ( "ALERT:Warning: You are in Incognito mode, data for this session will not be saved" ));

		mFileSystemOpened = true;
	}
	else {
		mFileSystemOpened = true;
	}
}

//----------------------------------------------------------------//
NaClFile * NaClFileSystem::fopen ( const char * path, const char *mode ) {

	//AJV support binary and text read/write? nacl file system makes no distinction, until then...
	//int modeLen = strlen ( mode );

	//AJV hack to differentiate local files from hosted
	char *NaClFilePath = strstr ( path, "/NaClFileSys" );
	const char *redirectPath = path;
	bool isOnDisk = false;

	if ( NaClFilePath ) {
		redirectPath = path + strlen ( "/NaClFileSys" );
		isOnDisk = true;
	}

	NaClFile * newFile = new NaClFile ();
	newFile->mPath = redirectPath;
	
	NACL_LOG ( "NaClFileSystem::fopen %s, mode %s, is on disk %d \n", newFile->mPath, mode, isOnDisk );

	if ( mCore->IsMainThread () ) {
		NACL_LOG( "ERROR: Cannot perform blocking file I/O on main thread\n" );
	}

	if ( mode[0] == 'r' ) {

		if ( isOnDisk ) {
			newFile->mFlags = PP_FILEOPENFLAG_READ;
		}
		else {
			pp::CompletionCallback cc ( RequestURLMainThread, newFile );

			if ( !mCore->IsMainThread ()) {
				mCore->CallOnMainThread ( 0, cc , 0 );
			}
			else {
				RequestURLMainThread ( newFile, 0 );
				newFile->mIsHttpLoaded = true;
			}

			while ( !newFile->mIsHttpLoaded ) {

				usleep ( 1000 );
			}
		}
	} 
	else if ( mode[0] == 'w' ) {

		if ( isOnDisk ) {
			newFile->mFlags = PP_FILEOPENFLAG_WRITE | PP_FILEOPENFLAG_CREATE | PP_FILEOPENFLAG_TRUNCATE;
		}
		else {
			NACL_LOG ( "ERROR: Cannot write to file not on local file system\n" );
		}

	}
	else {
		NACL_LOG ( "NaClFileSystem::fopen - Unsupported mode %s\n", mode );
	}

	if ( isOnDisk ) {

		pp::CompletionCallback cc ( OpenFileMainThread, newFile );

		newFile->mIsFileLocked = true;

		if ( !mCore->IsMainThread ()) {
			mCore->CallOnMainThread ( 0, cc , 0 );
		}
		else {
			OpenFileMainThread ( newFile, 0 );
		}

		while ( newFile->mIsFileLocked ) {

			usleep ( 1000 );
		}

		if ( newFile->mIsFileOpen ) {
			newFile->mIsFileExist = true;
		}

		//AJV read the entire file into memory :(
		//TODO not this.
		if ( newFile->mIsFileOpen && newFile->mFlags == PP_FILEOPENFLAG_READ ) {

			//get the size
			newFile->mIsFileLocked = true;

			pp::CompletionCallback ccSetSize ( SetSizeFileMainThread, newFile );

			if ( !mCore->IsMainThread ()) {
				mCore->CallOnMainThread ( 0, ccSetSize , 0 );
			}
			else {
				SetSizeFileMainThread ( newFile, 0 );
			}

			while ( newFile->mIsFileLocked ) {
				usleep ( 1000 );
			}

			newFile->mSize = newFile->mFileInfo.size;

			//now actually read it in
			newFile->mIsFileLocked = true;
			newFile->mData = new char [ newFile->mSize ];

			pp::CompletionCallback ccRead ( ReadFileMainThread, newFile );
		
			if ( !mCore->IsMainThread ()) {
				mCore->CallOnMainThread ( 0, ccRead , 0 );
			}
			else {
				ReadFileMainThread ( newFile, 0 );
				newFile->mIsFileLocked = false;
			}

			while ( newFile->mIsFileLocked ) {
				usleep ( 1000 );
			}

			newFile->mOffset = 0;

			NACL_LOG( "NaClFileSystem Read in file %s with %d bytes\n", path, newFile->mSize );
		}

	}

	if ( newFile->mIsFileExist ) {
		return newFile;
	}
	else {
		return NULL;
	}
}

//----------------------------------------------------------------//
void NaClFileSystem::RequestURLMainThread ( void * userData, int32_t result ) {
  
	NaClFile * file = static_cast < NaClFile * > ( userData );
	GetURLHandler* handler = GetURLHandler::Create( Get ()->mInstance, file->mPath );

	handler->SetUserData ( file );
	if (handler != NULL) {

		handler->Start( HttpLoaded );
	}
}

//----------------------------------------------------------------//
void NaClFileSystem::OpenFileMainThread ( void * userData, int32_t result ) {
  
	NaClFile * file = static_cast < NaClFile * > ( userData );
	
	pp::CompletionCallback cc ( OpenFileCallback, file );

	file->mFileRef = new pp::FileRef ( mSingletonInstance->mFileSystem, file->mPath );
	file->mFileIO = new pp::FileIO ( mSingletonInstance->mInstance );

	file->mFileIO->Open ( *( file->mFileRef ), file->mFlags, cc );
}

//----------------------------------------------------------------//
void NaClFileSystem::OpenFileCallback ( void * userData, int32_t result ) {
  
	NaClFile * file = static_cast < NaClFile * > ( userData );
	
	if ( result == PP_OK ) {
		file->mIsFileOpen = true;
	}

	file->mIsFileLocked = false;
}

//----------------------------------------------------------------//
int NaClFileSystem::stat ( const char *path, struct stat *buf ) {

	NaClFile * newFile = new NaClFile ();
	newFile->mPath = path;

	//TODO, check if file is cached
	pp::CompletionCallback cc ( RequestURLStatsMainThread, newFile );

	if ( !mCore->IsMainThread ()) {
		mCore->CallOnMainThread ( 0, cc , 0 );
	}
	else {
		NACL_LOG( "ERROR: Cannot load files on main thread\n" );
		RequestURLStatsMainThread ( newFile, 0 );
		newFile->mIsHttpLoaded = true;
	}

	while ( !newFile->mIsHttpLoaded ) {

		usleep ( 1000 );
	}

	if ( newFile->mIsFileExist ) {
		delete newFile;
		return 0;
	}
	else {
		delete newFile;
		return -1;
	}
}

void NaClFileSystem::SetSizeFileMainThread ( void * userData, int32_t result ) {

	NaClFile * file = static_cast < NaClFile * > ( userData );

	pp::CompletionCallback cc ( FileOperationDone, file );

	file->mFileIO->Query ( &file->mFileInfo, cc );
}

//----------------------------------------------------------------//
void NaClFileSystem::RequestURLStatsMainThread ( void * userData, int32_t result ) {
  
	NaClFile * file = static_cast < NaClFile * > ( userData );

	GetURLHandler* handler = GetURLHandler::Create( Get ()->mInstance, file->mPath );
	handler->SetMethod ( GetURLHandler::HEAD );
	handler->SetUserData ( file );

	if (handler != NULL) {

		handler->Start( HttpLoaded );
	}

}

//----------------------------------------------------------------//
int NaClFileSystem::fclose ( NaClFile *file ) {

	NACL_LOG( "NaClFileSystem::fclose %s \n\n", file->mPath );

	file->mIsFileLocked = true;

	if ( !mCore->IsMainThread ()) {
		pp::CompletionCallback cc ( DeletePepperFileMainThread, file );
		mCore->CallOnMainThread ( 0, cc , 0 );
	}
	else {
		DeletePepperFileMainThread ( file, 0 );
		file->mIsFileLocked = false;
	}

	while ( file->mIsFileLocked ) {
		usleep ( 1000 );
	}

	delete file;
	return 0;
}

void NaClFileSystem::DeletePepperFileMainThread ( void * userData, int32_t result ) {

	NaClFile * file = static_cast < NaClFile * > ( userData );

	if ( file->mFileRef ) {
		delete file->mFileRef;
	}

	if ( file->mFileIO ) {
		delete file->mFileIO;
	}

	file->mIsFileLocked = false;
}

//----------------------------------------------------------------//
size_t NaClFileSystem::fread ( void *ptr, size_t size, size_t count, NaClFile *file ) {

	if( file && ( file->mIsHttpLoaded || file->mIsFileOpen )) {

		int readSize = size * count;
		int remainingSize = file->mSize - file->mOffset;

		if ( readSize <= remainingSize ) {
			memcpy ( ptr, file->mData + file->mOffset, readSize );
			file->mOffset += readSize;
			return readSize;
		}
		else {
			memcpy ( ptr, file->mData + file->mOffset, remainingSize );
			file->mOffset += remainingSize;
			return remainingSize;
		}
	}
	//AJV unused for now due to complications with other std file I/O
	//e.g. getc, ungetc
	/*else if ( file && file->mIsFileOpen ) {

		NACL_LOG( "NaClFileSystem::fread filesys read\n" );
		file->mIsFileLocked = true;
		file->mExternalBuffer = ( char* ) ptr;
		file->mSize = size *  count;

		pp::CompletionCallback cc ( ReadFileMainThread, file );
		
		if ( !mCore->IsMainThread ()) {
			mCore->CallOnMainThread ( 0, cc , 0 );
		}
		else {
			NACL_LOG( "ERROR: Cannot read files on main thread\n" );
			ReadFileMainThread ( file, 0 );
			file->mIsFileLocked = false;
		}

		while ( file->mIsFileLocked ) {
			usleep ( 1000 );
		}

		file->mExternalBuffer = NULL;
		file->mOffset += size *  count;
	}*/
	else {
		NACL_LOG( "NaClFileSystem::fread - invalid file" );
	}

	return 0;
}

size_t NaClFileSystem::fwrite ( const void *ptr, size_t size, size_t count, NaClFile *file ) {

	if( file && file->mIsFileOpen ) {

		file->mIsFileLocked = true;
		file->mExternalBuffer = ( char* ) ptr;
		file->mSize = size *  count;

		pp::CompletionCallback cc ( WriteFileMainThread, file );
		
		if ( !mCore->IsMainThread ()) {
			mCore->CallOnMainThread ( 0, cc , 0 );
		}
		else {
			NACL_LOG ( "ERROR: Cannot write files on main thread\n" );
			WriteFileMainThread ( file, 0 );
			file->mIsFileLocked = false;
		}

		while ( file->mIsFileLocked ) {
			usleep ( 1000 );
		}

		file->mExternalBuffer = NULL;
		file->mOffset += size *  count;
	}

	return 0;
}

//----------------------------------------------------------------//
void NaClFileSystem::WriteFileMainThread ( void * userData, int32_t result ) {

	NaClFile * file = static_cast < NaClFile * > ( userData );

	pp::CompletionCallback cc ( FileOperationDone, file );
	file->mFileIO->Write ( file->mOffset, file->mExternalBuffer, file->mSize, cc );
}

//----------------------------------------------------------------//
void NaClFileSystem::ReadFileMainThread ( void * userData, int32_t result ) {

	NaClFile * file = static_cast < NaClFile * > ( userData );

	pp::CompletionCallback cc ( FileOperationDone, file );
	file->mFileIO->Read ( file->mOffset, file->mData, file->mSize, cc );
}

//----------------------------------------------------------------//
void NaClFileSystem::FileOperationDone ( void * userData, int32_t result )  {

	NaClFile * file = static_cast < NaClFile * > ( userData );

	file->mIsFileLocked = false;
}

//----------------------------------------------------------------//
void NaClFileSystem::HttpLoaded ( GetURLHandler *handler, const char *buffer, int32_t size ) {

	NaClFile *file = static_cast < NaClFile * > ( handler->GetUserData ());

	if ( handler->GetStatusCode () == 200 ) {
		file->mIsFileExist = true;
	}
	else {
		file->mIsFileExist = false;
	}

	if ( file->mIsFileExist && size ) {

		NACL_LOG ( "NaClFileSystem::HttpLoaded file %s, size: %d\n", file->mPath, size );

		file->mSize = size;
		file->mData = new char [ size ];

		memcpy ( file->mData, buffer, size );
	}

	file->mIsHttpLoaded = true;
}

//----------------------------------------------------------------//
NaClFileSystem * NaClFileSystem::Get () {

	return mSingletonInstance;
}

NaClFile::NaClFile () {

	mData = NULL;
	mIsFileExist = false;
	mFileRef = NULL;
	mFileIO = NULL;
	mFlags = 0;
	mIsFileLocked = false;
	mIsFileOpen = false;
	mIsHttpLoaded = false;
	mOffset = 0;
	mSize = 0;
}

NaClFile::~NaClFile () {
	mIsHttpLoaded = false;
	mSize = 0;
	if ( mData ) {
		delete [] mData;
	}

	mOffset = 0;
	mIsFileExist = false;
}
