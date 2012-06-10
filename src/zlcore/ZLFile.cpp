// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifdef NACL
	#include "NaClFile.h"
#endif
#include <zlcore/zl_util.h>
#include <zlcore/ZLFile.h>
#include <zlcore/ZLFileSystem.h>
#include <zlcore/ZLVirtualPath.h>
#include <zlcore/ZLZipStream.h>

#ifdef MOAI_COMPILER_MSVC
	#include <Share.h>
#endif

using namespace std;

//================================================================//
// ZLFile
//================================================================//

//----------------------------------------------------------------//
void ZLFile::ClearError () {

	if ( !this->mIsZip ) {
		clearerr ( this->mPtr.mFile );
	}
}

//----------------------------------------------------------------//
int	ZLFile::Close () {

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
int ZLFile::CloseProcess () {
	
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
int	ZLFile::Flush () {
	
	if ( !this->mIsZip ) {
		return fflush ( this->mPtr.mFile );
	}
	return 0;
}

//----------------------------------------------------------------//
int ZLFile::GetChar () {
	
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
int ZLFile::GetError () {

	return ( this->mIsZip ) ? 0 : ferror ( this->mPtr.mFile ); // TODO: error flag for zip file
}

//----------------------------------------------------------------//
int	ZLFile::GetFileNum () {
	
	// TODO:
	if ( !this->mIsZip ) {
		return fileno ( this->mPtr.mFile );
	}
	return -1;
}

//----------------------------------------------------------------//
int ZLFile::GetPos ( fpos_t* position ) {
	(( void )position );

	assert ( 0 ); // not implemented
	return -1;
}

//----------------------------------------------------------------//
char* ZLFile::GetString ( char* string, int length ) {
	
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
int ZLFile::IsEOF () {

	return ( this->mIsZip ) ? this->mPtr.mZip->IsAtEnd () : feof ( this->mPtr.mFile );
}

//----------------------------------------------------------------//
int ZLFile::Open ( const char* filename, const char* mode ) {
	
	ZLVirtualPath* mount;

	string abspath = ZLFileSystem::Get ().GetAbsoluteFilePath ( filename );
	filename = abspath.c_str ();
	
	mount = ZLFileSystem::Get ().FindBestVirtualPath ( filename );

	if ( mount ) {
		
		if ( mode [ 0 ] == 'r' ) {
		
			ZLZipStream* zipStream = 0;
			
			filename = mount->GetLocalPath ( filename );

			if ( filename ) {
				zipStream = ZLZipStream::Open ( mount->mArchive, filename );
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
			stdFile = _fsopen ( filename, mode, _SH_DENYNO );
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
int ZLFile::OpenProcess ( const char *command, const char *mode ) {

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
int ZLFile::OpenTemp () {

	this->mPtr.mFile = tmpfile ();
	return this->mPtr.mFile ? 0 : -1;
}

//----------------------------------------------------------------//
int ZLFile::PutChar ( int c ) {

	if ( !this->mIsZip ) {
		return fputc ( c, this->mPtr.mFile );
	}
	return EOF;
}

//----------------------------------------------------------------//
int ZLFile::PutString ( const char* string ) {

	if ( !this->mIsZip ) {
		return fputs ( string, this->mPtr.mFile );
	}
	return EOF;
}

//----------------------------------------------------------------//
size_t ZLFile::Read ( void* buffer, size_t size, size_t count ) {
	
	if ( this->mIsZip ) {
		size_t result = ( size_t )this->mPtr.mZip->Read ( buffer, size * count );
		return ( size_t )( result / size );
	}
	return fread ( buffer, size, count, this->mPtr.mFile );
}

//----------------------------------------------------------------//
int ZLFile::Reopen ( const char* filename, const char* mode ) {

	if ( this->mIsZip ) {
		this->Close ();
		return this->Open ( filename, mode );
	}
	else {

		FILE* stdFile = freopen ( filename, mode, this->mPtr.mFile );
		
		if ( stdFile ) {
			this->mPtr.mFile = stdFile;
			return 0;
		}
	}
	return -1;
}

//----------------------------------------------------------------//
int	ZLFile::Seek ( long offset, int origin ) {
	
	return ( this->mIsZip ) ? this->mPtr.mZip->Seek ( offset, origin ) : fseek ( this->mPtr.mFile, offset, origin );
}

//----------------------------------------------------------------//
void ZLFile::SetFile ( FILE* file ) {

	this->Flush ();
	this->mPtr.mFile = file;
	this->mIsZip = false;
}

//----------------------------------------------------------------//
int ZLFile::SetPos ( const fpos_t * pos ) {
	(( void )pos );

	assert ( 0 ); // not implemented
	return -1;
}

//----------------------------------------------------------------//
long ZLFile::Tell () {

	return ( this->mIsZip ) ? ( long )this->mPtr.mZip->Tell () : ftell ( this->mPtr.mFile );
}

//----------------------------------------------------------------//
size_t ZLFile::Write ( const void* data, size_t size, size_t count ) {
	
	if ( !this->mIsZip ) {
		return fwrite ( data, size, count, this->mPtr.mFile );
	}
	return 0;
}

//----------------------------------------------------------------//
void ZLFile::SetBuf ( char* buffer ) {

	if ( !this->mIsZip ) {
		setbuf ( this->mPtr.mFile, buffer );
	}
}

//----------------------------------------------------------------//
int ZLFile::SetVBuf ( char* buffer, int mode, size_t size ) {

	if ( !this->mIsZip ) {
		setvbuf ( this->mPtr.mFile, buffer, mode, size );
	}
	return 0;
}

//----------------------------------------------------------------//
int ZLFile::UnGetChar ( int character ) {
	
	if ( this->mIsZip ) {
		return this->mPtr.mZip->UnGetChar (( char )character ) ? EOF : 0;
	}
	return ungetc ( character, this->mPtr.mFile );
}

//----------------------------------------------------------------//
int ZLFile::VarPrintf ( const char* format, va_list arg ) {

	if ( !this->mIsZip ) {
		return vfprintf ( this->mPtr.mFile, format, arg );
	}
	return -1;
}

//----------------------------------------------------------------//
ZLFile::ZLFile () :
	mIsZip ( false ) {
	
	this->mPtr.mFile = 0;
	this->mPtr.mZip = 0;
}

//----------------------------------------------------------------//
ZLFile::~ZLFile () {

	this->Close ();
}
