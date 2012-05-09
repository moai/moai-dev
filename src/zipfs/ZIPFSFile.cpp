// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zipfs/zipfs_util.h>
#include <zipfs/ZIPFSFile.h>
#include <zipfs/ZIPFSFileSystem.h>
#include <zipfs/ZIPFSVirtualPath.h>
#include <zipfs/ZIPFSZipStream.h>

using namespace std;

//================================================================//
// ZIPFSFile
//================================================================//

//----------------------------------------------------------------//
void ZIPFSFile::ClearError () {

	if ( !this->mIsZip ) {
		clearerr ( this->mPtr.mFile );
	}
}

//----------------------------------------------------------------//
int	ZIPFSFile::Close () {

	int result = 0;

	if ( this->mIsZip ) {
		if ( this->mPtr.mZip ) {
			delete this->mPtr.mZip;
			this->mPtr.mZip = 0;
		}
	}
	else if ( this->mPtr.mFile ) {
		result = fclose ( this->mPtr.mFile );
		this->mPtr.mFile = 0;
	}
	return result;
}

//----------------------------------------------------------------//
int ZIPFSFile::CloseProcess () {
	
	FILE* stdFile = 0;
	if ( !this->mIsZip ) {
		stdFile = this->mPtr.mFile;
		this->mPtr.mFile = 0;
	}
	
	#ifdef MOAI_OS_WINDOWS
		return _pclose ( stdFile );
	#else
		return pclose ( stdFile );
	#endif
}

//----------------------------------------------------------------//
int	ZIPFSFile::Flush () {
	
	if ( !this->mIsZip ) {
		return fflush ( this->mPtr.mFile );
	}
	return 0;
}

//----------------------------------------------------------------//
int ZIPFSFile::GetChar () {
	
	int result = EOF;
		
	if ( this->mIsZip ) {
		char c;
		result = ( int )this->mPtr.mZip->Read ( &c, 1 );
		
		if ( result == 1 ) {
			result = c;
		}
	}
	else {
		result = fgetc ( this->mPtr.mFile );
	}
	return result;
}

//----------------------------------------------------------------//
int ZIPFSFile::GetError () {

	return ( this->mIsZip ) ? 0 : ferror ( this->mPtr.mFile ); // TODO: error flag for zip file
}

//----------------------------------------------------------------//
int	ZIPFSFile::GetFileNum () {
	
	// TODO:
	if ( !this->mIsZip ) {
		return fileno ( this->mPtr.mFile );
	}
	return -1;
}

//----------------------------------------------------------------//
int ZIPFSFile::GetPos ( fpos_t* position ) {
	(( void )position );

	assert ( 0 ); // not implemented
	return -1;
}

//----------------------------------------------------------------//
char* ZIPFSFile::GetString ( char* string, int length ) {
	
	int i = 0;
	int c = 0;
	
	if ( this->mIsZip ) {
	
		if ( length <= 1 ) return 0;

		do {
			c = this->GetChar ();
			if ( c == ( int )EOF || c == ( int )NULL ) break;
			
			string [ i++ ] = ( char )c;
			if ( i >= length ) return 0;
			
		} while ( c && ( c != '\n' ));

		if ( i == 0 ) {
			return 0;
		}
		string [ i ] = 0;
		return string;
	}

	return fgets ( string, length, this->mPtr.mFile );
}

//----------------------------------------------------------------//
int ZIPFSFile::IsEOF () {

	return ( this->mIsZip ) ? this->mPtr.mZip->IsAtEnd () : feof ( this->mPtr.mFile );
}

//----------------------------------------------------------------//
int ZIPFSFile::Open ( const char* filename, const char* mode ) {
	
	ZIPFSVirtualPath* mount;

	string abspath = ZIPFSFileSystem::Get ().GetAbsoluteFilePath ( filename );
	filename = abspath.c_str ();
	
	mount = ZIPFSFileSystem::Get ().FindBestVirtualPath ( filename );

	if ( mount ) {
		
		if ( mode [ 0 ] == 'r' ) {
		
			ZIPFSZipStream* zipStream = 0;
			
			filename = mount->GetLocalPath ( filename );

			if ( filename ) {
				zipStream = ZIPFSZipStream::Open ( mount->mArchive, filename );
			}
			
			if ( zipStream ) {
				this->mIsZip = 1;
				this->mPtr.mZip = zipStream;
				return 0;
			}
		}
	}
	else {

		#ifdef MOAI_COMPILER_MSVC
			FILE* stdFile = 0;
			_set_errno ( fopen_s ( &stdFile, filename, mode ));
		#else
			FILE* stdFile = fopen ( filename, mode );
		#endif
		
		if ( stdFile ) {
			this->mPtr.mFile = stdFile;
			return 0;
		}
	}
	return -1;
}

//----------------------------------------------------------------//
int ZIPFSFile::OpenProcess ( const char *command, const char *mode ) {

	FILE* stdFile = 0;

	#ifdef MOAI_OS_WINDOWS
		stdFile = _popen ( command, mode );	
	#else
		stdFile = popen ( command, mode );
	#endif

	if ( stdFile ) {
		this->mPtr.mFile = stdFile;
		return 0;
	}
	return -1;
}

//----------------------------------------------------------------//
int ZIPFSFile::OpenTemp () {

	this->mPtr.mFile = tmpfile ();
	return this->mPtr.mFile ? 0 : -1;
}

//----------------------------------------------------------------//
int ZIPFSFile::PutChar ( int c ) {

	if ( !this->mIsZip ) {
		return fputc ( c, this->mPtr.mFile );
	}
	return EOF;
}

//----------------------------------------------------------------//
int ZIPFSFile::PutString ( const char* string ) {

	if ( !this->mIsZip ) {
		return fputs ( string, this->mPtr.mFile );
	}
	return EOF;
}

//----------------------------------------------------------------//
size_t ZIPFSFile::Read ( void* buffer, size_t size, size_t count ) {
	
	if ( this->mIsZip ) {
		size_t result = ( size_t )this->mPtr.mZip->Read ( buffer, size * count );
		return ( size_t )( result / size );
	}
	return fread ( buffer, size, count, this->mPtr.mFile );
}

//----------------------------------------------------------------//
int ZIPFSFile::Reopen ( const char* filename, const char* mode ) {

	this->Close ();
	return this->Open ( filename, mode );
}

//----------------------------------------------------------------//
int	ZIPFSFile::Seek ( long offset, int origin ) {
	
	return ( this->mIsZip ) ? this->mPtr.mZip->Seek ( offset, origin ) : fseek ( this->mPtr.mFile, offset, origin );
}

//----------------------------------------------------------------//
void ZIPFSFile::SetFile ( FILE* file ) {

	this->mPtr.mFile = file;
	this->mIsZip = false;
}

//----------------------------------------------------------------//
int ZIPFSFile::SetPos ( const fpos_t * pos ) {
	(( void )pos );

	assert ( 0 ); // not implemented
	return -1;
}

//----------------------------------------------------------------//
long ZIPFSFile::Tell () {

	return ( this->mIsZip ) ? ( long )this->mPtr.mZip->Tell () : ftell ( this->mPtr.mFile );
}

//----------------------------------------------------------------//
size_t ZIPFSFile::Write ( const void* data, size_t size, size_t count ) {
	
	if ( !this->mIsZip ) {
		return fwrite ( data, size, count, this->mPtr.mFile );
	}
	return 0;
}

//----------------------------------------------------------------//
void ZIPFSFile::SetBuf ( char* buffer ) {

	if ( !this->mIsZip ) {
		setbuf ( this->mPtr.mFile, buffer );
	}
}

//----------------------------------------------------------------//
int ZIPFSFile::SetVBuf ( char* buffer, int mode, size_t size ) {

	if ( !this->mIsZip ) {
		setvbuf ( this->mPtr.mFile, buffer, mode, size );
	}
	return 0;
}

//----------------------------------------------------------------//
int ZIPFSFile::UnGetChar ( int character ) {
	
	if ( this->mIsZip ) {
		return this->mPtr.mZip->UnGetChar (( char )character ) ? EOF : 0;
	}
	return ungetc ( character, this->mPtr.mFile );
}

//----------------------------------------------------------------//
int ZIPFSFile::VarPrintf ( const char* format, va_list arg ) {

	if ( !this->mIsZip ) {
		return vfprintf ( this->mPtr.mFile, format, arg );
	}
	return -1;
}

//----------------------------------------------------------------//
ZIPFSFile::ZIPFSFile () :
	mIsZip ( false ) {
	
	this->mPtr.mFile = 0;
	this->mPtr.mZip = 0;
}

//----------------------------------------------------------------//
ZIPFSFile::~ZIPFSFile () {

	this->Close ();
}
