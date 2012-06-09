// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zlcore/ZLDirectoryItr.h>
#include <zlcore/ZLFileSystem.h>
#include <zlcore/ZLVirtualPath.h>

//================================================================//
// ZLDirectoryItr
//================================================================//

//----------------------------------------------------------------//
void ZLDirectoryItr::Close () {

	#ifdef _WIN32
		if ( this->mHandle ) {
			_findclose( this->mHandle );
		}
	#else
		if ( this->mHandle ) {
			closedir ( this->mHandle );
		}
	#endif
	
	this->mHandle = 0;
}

//----------------------------------------------------------------//
char const* ZLDirectoryItr::GetEntryName () {

	return this->mName.size () ? this->mName.c_str () : 0;
}

//----------------------------------------------------------------//
bool ZLDirectoryItr::IsSubdir () {

	return this->mIsDir;
}

//----------------------------------------------------------------//
int ZLDirectoryItr::Open () {

	ZLFileSystem& filesys = ZLFileSystem::Get ();

	this->mDirName = filesys.GetWorkingPath ();
	ZLVirtualPath* mount = filesys.FindBestVirtualPath ( this->mDirName.c_str ());
	
	if ( mount ) {
		char const* path = mount->GetLocalPath ( this->mDirName.c_str ());
		this->mZipFileDir = mount->mArchive->FindDir ( path );
	}
	else {
		this->mVirtualSubDir = filesys.FindNextVirtualSubdir ( this->mDirName.c_str (), 0 );
	}
	
	#ifndef _WIN32
		this->mHandle = opendir ( "." );
	#endif
	
	return 0;
}

//----------------------------------------------------------------//
int ZLDirectoryItr::ReadEntry () {

	this->mName.clear ();
	this->mIsDir = false;

	if ( this->mZipFileDir ) {
		return this->ReadZipEntry ();
	}
	
	if ( this->mVirtualSubDir ) {
	
		this->mName = this->mVirtualSubDir->mName.c_str ();
		this->mIsDir = true;
		this->mVirtualSubDir = ZLFileSystem::Get ().FindNextVirtualSubdir ( this->mDirName.c_str (), this->mVirtualSubDir );
		
		return 1;
	}
	
	#ifdef _WIN32
	{
		struct _finddata_t	fileInfo;
	
		if ( this->mHandle ) {
			if ( _findnext( this->mHandle, &fileInfo ) < 0 ) return 0;
		}
		else {
			this->mHandle = _findfirst ( "*", &fileInfo );
		}
		
		if ( this->mHandle ) {
			this->mName = fileInfo.name;
			this->mIsDir = ( fileInfo.attrib & _A_SUBDIR ) ? true : false;
			return 1;
		}
	}
	#else
	{
		struct dirent* entry = readdir ( this->mHandle );
		if ( entry ) {
			this->mName = entry->d_name;
			#ifndef NACL
				this->mIsDir = ( entry->d_type == DT_DIR ) ? true : false;
			#else
				this->mIsDir = true;
			#endif
			return 1;
		}
	}
	#endif
	
	return 0;
}

//----------------------------------------------------------------//
int ZLDirectoryItr::ReadZipEntry () {

	// only perform iteration if we have a valid directory
	// directory will be set to nil when iteration is finished
	if ( this->mZipFileDir ) {

		// iterate through the sub directories first
		// then the file entries

		if ( this->mZipFileSubDir ) {
			
			// we have a valid sub dir iterator, so iterate through
			// when iteration ends, move on to files
			
			this->mZipFileSubDir = this->mZipFileSubDir->mNext;
			if ( !this->mZipFileSubDir ) {
			
				// no more sub directories; move on to files
				this->mZipFileEntry = this->mZipFileDir->mChildFiles;
				if ( !this->mZipFileEntry ) {
				
					// no files, so end all iteration
					this->mZipFileDir = 0;
				}
			}
		}
		else if ( this->mZipFileEntry  ) {
			
			// try the next file in the iteration
			this->mZipFileEntry = this->mZipFileEntry->mNext;
			if ( !this->mZipFileEntry ) {
			
				// no more files, so end all iteration
				this->mZipFileDir = 0;
			}
		}
		else  {
			
			// there is no valid directory or file iterator, so begin iteration
			
			// first see if we have an valid sub directories
			this->mZipFileSubDir = this->mZipFileDir->mChildDirs;
			if ( !this->mZipFileSubDir ) {
		
				// no valid sub directories, so try files
				this->mZipFileEntry = this->mZipFileDir->mChildFiles;
				if ( !this->mZipFileEntry ) {
				
					// no valid files, so we're done
					this->mZipFileDir = 0;
				}
			}
		}
	}
	
	// if we have a valid dir at this point, it means we are still iterating
	if ( this->mZipFileDir ) {
		
		// get the name of the current directory or file and set the isDir flag accordingly
		if ( this->mZipFileSubDir ) {
			this->mName = this->mZipFileSubDir->mName.c_str ();
			this->mIsDir = true;
		}
		else if ( this->mZipFileEntry  ) {
			this->mName = this->mZipFileEntry->mName.c_str ();
			this->mIsDir = false;
		}
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
ZLDirectoryItr::ZLDirectoryItr () :
	mZipFileDir ( 0 ),
	mZipFileSubDir ( 0 ),
	mZipFileEntry ( 0 ),
	mVirtualSubDir ( 0 ),
	mIsDir ( false ),
	mHandle ( 0 ) {
}

//----------------------------------------------------------------//
ZLDirectoryItr::~ZLDirectoryItr () {

	this->Close ();
}