// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/uslscore.h>

extern "C" {
	#include <zlib.h>
	#include <zipfs/ZIPFSZipFile.h>
}

//----------------------------------------------------------------//
static void _cleanup () {

	USGlobals::Get ()->Finalize ();
	zipfs_cleanup ();
}

//----------------------------------------------------------------//
// TODO: this should be part of the unit tests
static void _typeCheck () {

	// make sure our fixed size typedefs are what we think
	// they are on the current platform/compiler
	assert ( sizeof ( cc8 )	== 1 );

	assert ( sizeof ( u8 )	== 1 );
	assert ( sizeof ( u16 )	== 2 );
	assert ( sizeof ( u32 )	== 4 );
	assert ( sizeof ( u64 )	== 8 );
	
	assert ( sizeof ( s8 )	== 1 );
	assert ( sizeof ( s16 )	== 2 );
	assert ( sizeof ( s32 )	== 4 );
	assert ( sizeof ( s64 )	== 8 );
}

//================================================================//
// uslscore
//================================================================//

//----------------------------------------------------------------//
void testMoaio () {

	//ZIPFSZipFile* zipFile = ZIPFSZipFile_New ( "test.zip" );
	//ZIPFSZipFile_Delete ( zipFile );

	//cc8* relpath;
	//
	//relpath = zipfs_get_rel_path ( "C:/foo/bar/baz.txt" );
	//relpath = zipfs_get_rel_path ( "/foo/bar/baz.txt" );
	//relpath = zipfs_get_rel_path ( "./foo/bar/baz.txt" );
	//relpath = zipfs_get_rel_path ( "foo/bar/baz.txt" );

	zipfs_mount_virtual ( "test", "test.zip" );
	zipfs_chdir ( "test//foo/baz////" );
	
	zipfs_rmdir ( "test" );
	zipfs_rmdir ( "test/foo" );
	
	ZIPFSDIR dir = zipfs_dir_open ();
	
	while ( zipfs_dir_read_entry ( dir )) {
		cc8* name = zipfs_dir_entry_name ( dir );
		int isSubDir = zipfs_dir_entry_is_subdir ( dir );
		
		printf ( "%s: %s\n", isSubDir ? "dir" : "file", name );
	}
	zipfs_dir_close ( dir );
	
	zipfs_chdir ( "b" );
	
	zipfs_stat filestat;
	zipfs_get_stat ( "././/./..//c////Metamorphosis.txt", &filestat );
	
	ZIPFSFILE file = zipfs_fopen ( "././/./..//c////Metamorphosis.txt", "r" );
	
	char buffer [ 1024 ];
	
	zipfs_fread ( buffer, 1024, 1, file );
	zipfs_fseek ( file, -512, SEEK_CUR );
	
	zipfs_fread ( buffer, 1024, 1, file );
	buffer [ 1023 ] = 0;
	printf ( "%s\n", buffer );
	
	zipfs_fclose ( file );
}

//----------------------------------------------------------------//
void uslscore::InitGlobals ( USGlobals* globals ) {

	static bool sysInit = true;
	if ( sysInit ) {

		_typeCheck ();

		//#ifdef _WIN32
		//	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
		//#endif
		
		srand (( u32 )time ( 0 ));
		atexit ( _cleanup );
		
		zipfs_init ();
		
		testMoaio ();
		
		sysInit = false;
	}

	USGlobals::Get ()->Set ( globals );

	USLuaRuntime::Get ();
}
