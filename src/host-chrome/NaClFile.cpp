
#define NACLFILE_CPP
#include "NaClFile.h"
#include "NaClFileSystem.h"

//#include <stdio.h>
//#define EOF -1

FILE * nacl_fopen ( const char * path, const char *mode ) {

	return  ( FILE * )NaClFileSystem::Get ()->fopen ( path, mode );
}

int nacl_fclose ( FILE *file ) {

	return NaClFileSystem::Get ()->fclose ((NaClFile *) file );
}

int nacl_fread ( void *ptr, int size, int count, FILE *file ) {

	return NaClFileSystem::Get ()->fread ( ptr, size, count, (NaClFile *) file );
}

int nacl_fwrite ( const void * ptr, int size, int count, FILE *file ) {

	return NaClFileSystem::Get ()->fwrite ( ptr, size, count, (NaClFile *) file );
}

int nacl_feof ( FILE * void_file ) {

	NaClFile * file = ( NaClFile * ) void_file;
	return ( file->mOffset == file->mSize );
}

int nacl_ferror ( FILE * void_file ) {

	//unimplemented
	return 0;
}

void nacl_clearerr ( FILE * void_file ) {
	//do nothing
}

int nacl_fgetc ( FILE * void_file ) {

	NaClFile * file = ( NaClFile * ) void_file;

	if( file && file->mIsHttpLoaded ) {

		int remainingSize = file->mSize - file->mOffset;

		if ( remainingSize ) {
			int data = file->mData [ file->mOffset ];
			file->mOffset += 1;
			return data;
		}
		else {
			return EOF;
		}
	}
	return 0;
}

int nacl_ungetc (int c, FILE *void_file) {

	NaClFile * file = ( NaClFile * ) void_file;

	if( file && file->mIsHttpLoaded ) {

		if ( file->mOffset ) {
			file->mOffset -= 1;
			file->mData [ file->mOffset ] = c;
			return file->mData [ file->mOffset ];
		}
		else {
			return EOF;
		}
	}
	return 0;
}

int nacl_fseek ( FILE * void_file, long int offset, int origin ) {

	NaClFile * file = ( NaClFile * ) void_file;

	int originPosition;
	switch ( origin ) {
		case SEEK_SET:
			originPosition = 0; 
			break;
		case SEEK_CUR:
			originPosition = file->mOffset;
			break;
		case SEEK_END:
			originPosition = file->mSize;
			break;
	}
	int position = originPosition + offset;
	if ( position <= file->mSize ) {

		file->mOffset = position;
		return 0;
	}

	return -1;
}

long int nacl_ftell ( FILE * void_file ) {

	NaClFile * file = ( NaClFile * ) void_file;

	return file->mOffset;
}

int nacl_stat ( const char *path, struct stat *s ) {

	//open file with Head
	int foundFile = NaClFileSystem::Get ()->stat ( path, s );

	if ( !foundFile ) {

		s->st_mode = S_IFREG;
		s->st_ctime = 0;
		s->st_mtime = 0;
		s->st_atime = 0;

		return 0;
	}

	return -1;
}

char *nacl_getcwd(char *buf, size_t size) {

	for ( int i = 0; i < size; ++i ) {
		buf [ i ] = ( char ) 0;
	}

	buf [ 0 ] = '/';

	return buf;
}
