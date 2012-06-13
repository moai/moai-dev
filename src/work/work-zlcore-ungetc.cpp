#include <uslscore/uslscore.h>

//----------------------------------------------------------------//
int work_zl_ungetc ( int argc, char **argv ) {
	
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
	zl_init ();
	
	zl_mount_virtual ( "test", "ungetc.zip" );
	zl_chdir ( "test" );
	
	ZLFILE* zfile = zl_fopen ( "ungetc.txt", "r" );
	memset ( buffer, 0, 256 );
	
	zl_fread ( buffer, 1, 5, zfile );
	zl_ungetc ( '0', zfile );
	zl_ungetc ( '1', zfile );
	zl_ungetc ( '2', zfile );
	zl_ungetc ( '3', zfile );
	zl_ungetc ( '4', zfile );
	zl_fread ( buffer, 1, 3, zfile );
	zl_fseek ( zfile, 0, SEEK_CUR );
	zl_fread ( buffer, 1, 3, zfile );
	zl_fclose ( zfile );
	
	zl_cleanup ();
	
	return 0;
}
