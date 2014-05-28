// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <string.h>
#include <host-modules/aku_modules.h>
#include <host-modules/aku_modules_util.h>
#include <lua-headers/moai_lua.h>

//================================================================//
// implementation
//================================================================//

//----------------------------------------------------------------//
void AKUModulesParseArgs ( int argc, char** argv ) {

	#if AKU_WITH_TEST

		int total = argc - 1;
		int i = 1;

		for ( ; i < total; ++i ) {

			char* arg = argv [ i ];

			if ( arg [ 0 ] != '-' ) break;

			// filter file
			if ( strcmp ( arg, "-f" ) == 0 ) {
				AKUTestSetFilterFile ( argv [ ++i ]);
			}

			// filter
			if ( strcmp ( arg, "-F" ) == 0 ) {
				AKUTestSetFilter ( argv [ ++i ]);
			}

			// results
			if ( strcmp ( arg, "-r" ) == 0 ) {
				AKUTestSetResultsFile ( argv [ ++i ]);
			}

			// staging
			if ( strcmp ( arg, "-s" ) == 0 ) {
				AKUTestSetStaging ();
			}

			// test
			if ( strcmp ( arg, "-t" ) == 0 ) {
				AKUTestRunTest ( argv [ ++i ]);
			}

			// xml results
			if ( strcmp ( arg, "-x" ) == 0 ) {
				AKUTestSetXmlResultsFile ( argv [ ++i ]);
			}
		}

		for ( ; i < argc; ++i ) {
			AKUTestRunScript ( argv [ i ]);
		}

	#else

		if ( argc < 2 ) {
			AKURunScript ( "main.lua" );
		}
		else {

			AKUSetArgv ( argv );
			for ( int i = 1; i < argc; ++i ) {
				char* arg = argv [ i ];
				if (( strcmp ( arg, "-s" ) == 0 ) && ( ++i < argc )) {
					char* script = argv [ i ];
					AKURunString ( script );
				}
				else if (( strcmp ( arg, "-a" ) == 0 )) {
					if ( ++i >= argc )
						return;
					char* fileName = argv [ i ];
					arg = argv [ ++ i ];
					
					AKURunScript( fileName, arg );
				}
				else {
					AKURunScript ( arg );
				}
			}
		}

	#endif
}

//----------------------------------------------------------------//
void AKUModulesRunLuaAPIWrapper () {

	AKURunData ( moai_lua, moai_lua_SIZE, AKU_DATA_STRING, AKU_DATA_ZIPPED );
}
