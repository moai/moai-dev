#include <uslscore/uslscore.h>

//----------------------------------------------------------------//
int work_zipfs ( int argc, char **argv ) {

	ZIPFSFILE* file;
	char buffer [ 4096 ];

	zipfs_init ();

	// mount some virtual paths
	zipfs_mount_virtual ( "test1", "test.zip" );
	zipfs_mount_virtual ( "test2", "uncompressed.zip" );
	
	// these should fail
	zipfs_rmdir ( "test1" );
	zipfs_rmdir ( "test1/foo" );
	
	// navigate into the path
	zipfs_chdir ( "test1//foo/baz////" );
	
	// iterate through the contents of baz
	ZIPFSDIR* dir = zipfs_dir_open ();
	
	while ( zipfs_dir_read_entry ( dir )) {
		cc8* name = zipfs_dir_entry_name ( dir );
		int isSubDir = zipfs_dir_entry_is_subdir ( dir );
		
		printf ( "%s: %s\n", isSubDir ? "dir" : "file", name );
	}
	zipfs_dir_close ( dir );
	
	zipfs_chdir ( "b" );
	
	//----------------------------------------------------------------//
	// Metamorphosis
	zipfs_stat filestat;
	zipfs_get_stat ( "././/./..//c////Metamorphosis.txt", &filestat );
	
	// open file using relative path
	file = zipfs_fopen ( "././/./..//c////Metamorphosis.txt", "r" );
	
	zipfs_fread ( buffer, 1024, 1, file );
	zipfs_fseek ( file, -512, SEEK_CUR );
	zipfs_fread ( buffer, 1024, 1, file );
	zipfs_fclose ( file );
	
	//----------------------------------------------------------------//
	// Sherlock
	file = zipfs_fopen ( "../../../../test2/The Adventures of Sherlock Holmes.txt", "r" );
	
	zipfs_fread ( buffer, 4096, 1, file );
	zipfs_fseek ( file, 1024, SEEK_SET );
	zipfs_fread ( buffer, 4096, 1, file );
	zipfs_fclose ( file );
	
	zipfs_cleanup ();
	return 0;
}
