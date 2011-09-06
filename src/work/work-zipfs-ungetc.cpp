#include <uslscore/uslscore.h>

//----------------------------------------------------------------//
int work_zipfs_ungetc ( int argc, char **argv ) {
	
	char buffer [ 256 ];
	
	// try it straight
	FILE* file = fopen ( "ungetc.txt", "r" );
	memset ( buffer, 0, 256 );
	
	fread ( buffer, 1, 5, file );
	ungetc ( '0', file );
	ungetc ( '1', file );
	ungetc ( '2', file );
	ungetc ( '3', file );
	ungetc ( '4', file );
	fread ( buffer, 1, 3, file );
	fseek ( file, 0, SEEK_CUR );
	fread ( buffer, 1, 3, file );
	fclose ( file );
	
	// try it virtual
	zipfs_init ();
	
	zipfs_mount_virtual ( "test", "ungetc.zip" );
	zipfs_chdir ( "test" );
	
	ZIPFSFILE* zfile = zipfs_fopen ( "ungetc.txt", "r" );
	memset ( buffer, 0, 256 );
	
	zipfs_fread ( buffer, 1, 5, zfile );
	zipfs_ungetc ( '0', zfile );
	zipfs_ungetc ( '1', zfile );
	zipfs_ungetc ( '2', zfile );
	zipfs_ungetc ( '3', zfile );
	zipfs_ungetc ( '4', zfile );
	zipfs_fread ( buffer, 1, 3, zfile );
	zipfs_fseek ( zfile, 0, SEEK_CUR );
	zipfs_fread ( buffer, 1, 3, zfile );
	zipfs_fclose ( zfile );
	
	zipfs_cleanup ();
	
	return 0;
}
