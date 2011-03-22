// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USFilename.h>
#include <uslscore/USFileSys.h>

//================================================================//
// USFilename
//================================================================//

//----------------------------------------------------------------//
void USFilename::Append ( char c ) {

	this->mBuffer [ this->mLength ] = c;

	if ( USPathOps::IsSeparator ( c )) {
		this->mNameIdx = this->mLength + 1;
		this->mExtIdx = -1;
	}
		
	if ( c == '.' ) {
		this->mExtIdx = this->mLength;
	}

	this->mLength++;
	this->mBuffer [ this->mLength ] = 0;
}

//----------------------------------------------------------------//
void USFilename::Append ( cc8* str ) {

	if ( !str ) return;
	
	for ( u32 i = 0; str [ i ]; ++i ) {
		this->Append ( str [ i ]);
	}
}

//----------------------------------------------------------------//
char* USFilename::Bless ( cc8* path ) {

	int base = this->mLength;
	for ( int i = 0; path [ i ]; ++i ) {
		
		this->Append ( path [ i ]);
	}
	
	for ( int i = base; i < this->mNameIdx; ++i ) {
		this->mBuffer [ i ] = USPathOps::Bless ( this->mBuffer [ i ]);
	}
	
	if ( this->mExtIdx > this->mNameIdx ) {
		for ( int i = this->mExtIdx; this->mBuffer [ i ]; ++i ) {
			this->mBuffer [ i ] = USPathOps::Bless ( this->mBuffer [ i ]);
		}
	}
	
	return this->mBuffer;
}

//----------------------------------------------------------------//
void USFilename::Clear () {

	this->mLength = 0;
	this->mNameIdx = -1;
	this->mExtIdx = -1;

	mBuffer [ 0 ] = 0;
}

//----------------------------------------------------------------//
u32 USFilename::ComparePath ( USFilename& filename ) {

	int len = this->mLength;
	if ( filename.mLength < len ) len = filename.mLength;
	
	if ( len == 0 ) return 0;

	u32 same = 0;

	for ( int i = 0; i < len; ++i ) {
		
		char h = ( char )tolower ( this->mBuffer [ i ]);
		char v = ( char )tolower ( filename.mBuffer [ i ]);
		
		if ( h != v ) break;
		
		if ( h == '/' ) {
			same = ( u32 )i + 1;
		}
	}

	return same;
}

//----------------------------------------------------------------//
char* USFilename::ConvertToPath () {

	if ( this->mLength == 0 ) return this->mBuffer;
	if ( this->mBuffer [ this->mLength - 1 ] == '/' ) return this->mBuffer;

	assert ( this->mNameIdx );
	
	for ( int i = this->mNameIdx; this->mBuffer [ i ]; ++i ) {
		this->mBuffer [ i ] = USPathOps::Bless ( this->mBuffer [ i ]);
	}
	
	this->mNameIdx = -1;
	this->mExtIdx = -1;
	
	this->Append ( '/' );
	
	return this->mBuffer;
}

//----------------------------------------------------------------//
void USFilename::Copy ( USFilename& filename ) {

	this->mLength = filename.mLength;
	this->mNameIdx = filename.mNameIdx;
	this->mExtIdx = filename.mExtIdx;

	memcpy ( this->mBuffer, filename.mBuffer, this->mLength );
}

//----------------------------------------------------------------//
char* USFilename::Expand ( cc8* filename ) {

	this->Clear ();

	// handle absolute paths
	if ( !filename ) return ( char* )"/";
	if ( USPathOps::IsSeparator ( filename [ 0 ])) {
		return this->Bless ( filename );
	}
	
	if (( filename [ 1 ]) == ':' ) {
		return this->Bless ( filename );
	}

	// get the current path into a temp buffer
	USFilename temp;
	temp.GetCurrentPath ();
	
	// append the filename
	temp.Bless ( filename );
	
	// and normalize
	return this->Normalize ( temp.mBuffer );
}

//----------------------------------------------------------------//
char* USFilename::ExpandPath ( cc8* filename ) {

	this->Expand ( filename );
	return this->ConvertToPath ();
}

//----------------------------------------------------------------//
USFilename::USFilename () :
	mLength ( 0 ),
	mNameIdx ( -1 ),
	mExtIdx ( -1 ) {
	
	mBuffer [ 0 ] = 0;
}

//----------------------------------------------------------------//
USFilename::~USFilename () {
}

//----------------------------------------------------------------//
char* USFilename::GetCurrentPath () {

	this->Clear ();

	char path [ PATH_MAX ];
	char* str = getcwd ( path, PATH_MAX );
	UNUSED ( str );
	
	this->Bless ( path );
	this->ConvertToPath ();

	return this->mBuffer;
}

//----------------------------------------------------------------//
char* USFilename::GetExt ( cc8* filename ) {

	this->Bless ( filename );
	if ( this->mExtIdx < this->mLength ) {
		return &this->mBuffer [ this->mExtIdx ];
	}

	return ( char* )"";
}

//----------------------------------------------------------------//
char* USFilename::GetName ( cc8* filename ) {

	USFilename::Bless ( filename );
	if ( this->mNameIdx < this->mLength ) {
		return &this->mBuffer [ this->mNameIdx ];
	}

	return ( char* )"";
}

//----------------------------------------------------------------//
char* USFilename::GetNameNoExt ( cc8* filename ) {

	USFilename::Bless ( filename );
	if ( this->mNameIdx < this->mLength ) {
		this->mBuffer [ this->mExtIdx ] = 0;
		return &this->mBuffer [ this->mNameIdx ];
	}

	return ( char* )"";
}

//----------------------------------------------------------------//
char* USFilename::GetPath ( cc8* filename ) {

	USFilename::Bless ( filename );
	this->mBuffer [ this->mNameIdx ] = 0;
	return this->mBuffer;
}

//----------------------------------------------------------------//
char* USFilename::GetRelativePath ( cc8* path ) {
	
	this->GetCurrentPath ();

	USFilename fullpath;
	fullpath.Expand ( path );

	u32 same = this->ComparePath ( fullpath );
	if ( same == 0 ) {
		this->Clear ();
		this->Bless ( path );
		return this->mBuffer;
	}

	// count the number of steps up in the current directory
	u32 depth = 0;
	for ( u32 i = same; this->mBuffer [ i ]; ++i ) {
		if (  this->mBuffer [ i ] == '/' ) {
			depth++;
		}
	}

	this->Clear ();
	for ( u32 i = 0; i < depth; ++i ) {
		this->Append ( "../" );
	}

	this->Append ( &fullpath.mBuffer [ same ]);
	
	return this->mBuffer;
}

//----------------------------------------------------------------//
char* USFilename::Normalize ( cc8* path ) {

	this->Clear ();

	// normalize the path
	for ( int i = 0; path [ i ]; ++i ) {
	
		if ( path [ i ] == '.' ) {
		
			if ( path [ i + 1 ] == '/' ) {
				i += 1;
				continue;
			}
			
			if (( path [ i + 1 ] == '.' ) && ( path [ i + 2 ] == '/' )) {

				for ( int j = this->mLength; j > 0; --j ) {
					if ( this->mBuffer [ j ] == '/' ) {
						for ( int k = j - 1; k > 0; --k ) {
							
							if ( this->mBuffer [ k ] == '/' ) {
								this->mLength = k + 1;
								this->mNameIdx = this->mLength;
								this->mExtIdx = -1;
								this->mBuffer [ this->mLength ] = 0;
								break;
							}
						}
						break;
					}
				}
				i += 2;
				continue;
			}
		}
		
		this->Append ( path [ i ]);
	}
	
	return this->mBuffer;
}

//================================================================//
// USPathOps
//================================================================//

//----------------------------------------------------------------//
char USPathOps::Bless ( char c ) {

	if ( c == '\\' ) return '/';
	//return ( char )tolower ( c );
	return c;
}

//----------------------------------------------------------------//
STLString USPathOps::Bless ( cc8* path ) {

	USFilename fullpath;
	return fullpath.Bless ( path );
}

//----------------------------------------------------------------//
u32 USPathOps::ComparePaths ( cc8* p0, cc8* p1 ) {

	USFilename path0;
	path0.GetPath ( p0 );

	USFilename path1;
	path1.GetPath ( p1 );

	return path0.ComparePath ( path1 );
}

//----------------------------------------------------------------//
STLString USPathOps::GetExt ( cc8* filename ) {

	USFilename fullpath;
	return fullpath.GetExt ( filename );
}

//----------------------------------------------------------------//
STLString USPathOps::GetName ( cc8* filename ) {

	USFilename fullpath;
	return fullpath.GetName ( filename );
}

//----------------------------------------------------------------//
STLString USPathOps::GetNameNoExt ( cc8* filename ) {

	USFilename fullpath;
	return fullpath.GetNameNoExt ( filename );
}

//----------------------------------------------------------------//
STLString USPathOps::GetPath ( cc8* filename, bool expand ) {

	USFilename fullpath;
	
	if ( expand ) {
		fullpath.Expand ( filename );
		fullpath.mBuffer [ fullpath.mNameIdx ] = 0;
		return fullpath.mBuffer;
	}
	
	return fullpath.GetPath ( filename );
}

//----------------------------------------------------------------//
bool USPathOps::IsSeparator ( const char c ) {

	return (( c == '/' ) || ( c == '\\' ) || ( c == ':' ));
}


