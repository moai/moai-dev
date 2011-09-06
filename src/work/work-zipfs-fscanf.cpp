#include <uslscore/uslscore.h>

//----------------------------------------------------------------//
int work_zipfs_fscanf ( int argc, char **argv ) {
	
	float f1, f2;
	
	zipfs_init ();
	
	// try it straight
	
	FILE* file = fopen ( "scan.txt", "r" );
	fscanf ( file, "%f,    %f", &f1, &f2 );
	fclose ( file );
	
	// try it virtual
	zipfs_mount_virtual ( "scan", "scan.zip" );
	
	ZIPFSFILE* zfile = zipfs_fopen ( "scan/scan.txt", "r" );
	zipfs_fscanf ( zfile, "%f", &f1 );
	zipfs_fclose ( zfile );
	
	zipfs_cleanup ();
	return 0;
}
