#include <uslscore/uslscore.h>

//----------------------------------------------------------------//
int work_zlcore ( int argc, char **argv ) {

	ZLFILE* file;
	char buffer [ 4096 ];

	zl_init ();

	// mount some virtual paths
	zl_mount_virtual ( "test1", "test.zip" );
	zl_mount_virtual ( "test2", "uncompressed.zip" );
	
	// these should fail
	zl_rmdir ( "test1" );
	zl_rmdir ( "test1/foo" );
	
	// navigate into the path
	zl_chdir ( "test1//foo/baz////" );
	
	// iterate through the contents of baz
	ZLDIR* dir = zl_dir_open ();
	
	while ( zl_dir_read_entry ( dir )) {
		cc8* name = zl_dir_entry_name ( dir );
		int isSubDir = zl_dir_entry_is_subdir ( dir );
		
		printf ( "%s: %s\n", isSubDir ? "dir" : "file", name );
	}
	zl_dir_close ( dir );
	
	zl_chdir ( "b" );
	
	//----------------------------------------------------------------//
	// Metamorphosis
	zl_stat filestat;
	zl_get_stat ( "././/./..//c////Metamorphosis.txt", &filestat );
	
	// open file using relative path
	file = zl_fopen ( "././/./..//c////Metamorphosis.txt", "r" );
	
	zl_fread ( buffer, 1024, 1, file );
	zl_fseek ( file, -512, SEEK_CUR );
	zl_fread ( buffer, 1024, 1, file );
	zl_fclose ( file );
	
	//----------------------------------------------------------------//
	// Sherlock
	file = zl_fopen ( "../../../../test2/The Adventures of Sherlock Holmes.txt", "r" );
	
	zl_fread ( buffer, 4096, 1, file );
	zl_fseek ( file, 1024, SEEK_SET );
	zl_fread ( buffer, 4096, 1, file );
	zl_fclose ( file );
	
	zl_cleanup ();
	return 0;
}
