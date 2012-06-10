// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLFILE_H
#define ZLFILE_H

class ZLZipStream;

//================================================================//
// ZLFile
//================================================================//
class ZLFile {
private:

	bool	mIsZip;
	
	union {
		FILE*				mFile;
		ZLZipStream*		mZip;
	} mPtr;

public:

	//----------------------------------------------------------------//
	void			ClearError			();
	int				Close				();
	int				CloseProcess		();
	int				Flush				();
	int				GetChar				();
	int				GetError			();
	int				GetFileNum			();
	int				GetPos				( fpos_t* position );
	char*			GetString			( char* string, int length );
	int				IsEOF				();
	int				Open				( const char* filename, const char* mode );
	int				OpenProcess			( const char *command, const char *mode );
	int				OpenTemp			();
	int				PutChar				( int c );
	int				PutString			( const char* string );
	size_t			Read				( void* buffer, size_t size, size_t count );
	int				Reopen				( const char* filename, const char* mode );
	int				Seek				( long offset, int origin );
	void			SetFile				( FILE* file );
	int				SetPos				( const fpos_t * pos );
	long			Tell				();
	size_t			Write				( const void* data, size_t size, size_t count );
	void			SetBuf				( char* buffer );
	int				SetVBuf				( char* buffer, int mode, size_t size );
	int				UnGetChar			( int character );
	int				VarPrintf			( const char* format, va_list arg );
					ZLFile				();
					~ZLFile				();
};

#endif
