#ifndef NACLFILESYSTEM_H
#define NACLFILESYSTEM_H

#include <string>
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/file_system.h"
#include "ppapi/cpp/file_ref.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/file_io.h"
#include "ppapi/c/ppb_file_io.h"
#include "geturl_handler.h"

class NaClFile {
public:
	NaClFile ();
	~NaClFile ();

	char * mData;
	char * mExternalBuffer;
	
	pp::FileRef *mFileRef;
	pp::FileIO *mFileIO;
	PP_FileInfo mFileInfo;
	int mFlags;

	bool mIsFileExist;
	bool mIsFileLocked;
	bool mIsFileOpen;
	bool mIsHttpLoaded;

	int mOffset;
	const char * mPath;
	int mSize;
};


class NaClFileSystem {
public:

	NaClFileSystem ( pp::Core* core, pp::Instance *instance );
	~NaClFileSystem ();

	void Init ();

	static NaClFileSystem * Get ();

	//TODO - cache file to HD on open
	NaClFile * fopen ( const char * path, const char *mode );

	int fclose ( NaClFile *file );

	size_t fread ( void *ptr, size_t size, size_t count, NaClFile *a_file );
	size_t fwrite ( const void *ptr, size_t size, size_t count, NaClFile *a_file );

	int stat ( const char *path, struct stat *buf );

private:
	
	//main thread callback functions, C API since callback factory does not work on bg thread

	//URL
	static void RequestURLMainThread ( void* userData, int32_t result );
	static void RequestURLStatsMainThread ( void * userData, int32_t result );

	//File
	static void OpenFileMainThread ( void * userData, int32_t result );
	static void OpenFileCallback ( void * userData, int32_t result );

	static void SetSizeFileMainThread ( void * userData, int32_t result ); 
	static void WriteFileMainThread ( void * userData, int32_t result );
	static void ReadFileMainThread ( void * userData, int32_t result );
	static void FileOperationDone ( void * userData, int32_t result );

	static void DeletePepperFileMainThread ( void * userData, int32_t result );

	//pp Filesystem
	static void OpenFileSystemMainThread ( void* userData, int32_t result );

	void OpenFileSystemCallback ( int32_t result );

	//
	int QuerySize ( const char *file );

	pp::CompletionCallbackFactory<NaClFileSystem> mCCFactory;

	pp::Core* mCore;
	pp::Instance *mInstance;
	
	bool mFileSystemOpened;

	pp::FileSystem mFileSystem;

	static NaClFileSystem *mSingletonInstance;

	static void HttpLoaded ( GetURLHandler *handler, const char *buffer, int32_t size );

	//TODO, remove
	NaClFile mCurrentFile;
};

#endif
